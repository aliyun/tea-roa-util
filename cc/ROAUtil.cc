
#include <sstream>
#include <list>
#include <unordered_map>
#include <openssl/hmac.h>
#include "ROAUtil.h"

using namespace std;
using namespace ROAUtil;

bool Client::is4XXor5XX(int code) {
    return code >= 400 && code < 600;
}

string withDefault(string input, string defaultValue) {
    if (input.empty()) {
        return defaultValue;
    }
    return input;
}

void stringReplace(std::string & src,
  const std::string & s1, const std::string & s2) {
  std::string::size_type pos = 0;
  while ((pos = src.find(s1, pos)) != std::string::npos) {
    src.replace(pos, s1.length(), s2);
    pos += s2.length();
  }
}

string filterAndTrim(string input) {
    if (input.empty()) 
    {
        return input;
    }

    stringReplace(input, "\t", " ");
    stringReplace(input, "\n", " ");
    stringReplace(input, "\r", " ");
    stringReplace(input, "\f", " ");

    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    return input;
}

string getCanonicalizedHeaders(unordered_map<string, string> headers) {
  string prefix = "x-acs-";
  list<string> keys = { };
  for (auto &&item : headers)
  {
    keys.push_back(item.first);
  }
  keys.sort();

  list<string> canonicalizedKeys = { };
  for (auto &&key : keys)
  {
    if (key.compare(0, prefix.size(), prefix) == 0) {
      canonicalizedKeys.push_back(key);
    }
  }

  canonicalizedKeys.sort();

  stringstream result;
  for (auto &&key : canonicalizedKeys)
  {
      result << key << ":" << filterAndTrim(headers[key]) << "\n";
  }

  return result.str();
}

string getCanonicalizedResource(string uriPattern, unordered_map<string, string> query) {
  list<string> keys = { };
  for (auto &&item : query)
  {
    keys.push_back(item.first);
  }
  keys.sort();

  if (keys.size() == 0) {
    return uriPattern;
  }

  stringstream result;
  result << uriPattern << "?";
  for (auto &&key : keys)
  {
    result << key + "=" + query[key];
    if (key != keys.back()) {
      result << "&";
    }
  }

  return result.str();
}

string Client::getStringToSign(Tea::Request request) {
    string method = request.method;
    string accept = request.headers["accept"];
    string contentMD5 = withDefault(request.headers["content-md5"], "");
    string contentType = withDefault(request.headers["content-type"], "");
    string date = withDefault(request.headers["date"], "");
    stringstream stringToSign;
    stringToSign << method << "\n" << accept << "\n" << contentMD5 << "\n" << contentType << "\n" << date << "\n";
    stringToSign << getCanonicalizedHeaders(request.headers);
    stringToSign << getCanonicalizedResource(request.pathname, request.query);
    return stringToSign.str();
}

string Client::getSignature(string stringToSign, string secret) {
    if (stringToSign.empty()) {
        return string();
    }

#ifdef _WIN32
  typedef struct _my_blob {
    BLOBHEADER hdr;
    DWORD dwKeySize;
    BYTE rgbKeyData[];
  }my_blob;

  DWORD kbLen = sizeof(my_blob) + secret.size();
  my_blob * kb = (my_blob *)LocalAlloc(LPTR, kbLen);
  kb->hdr.bType = PLAINTEXTKEYBLOB;
  kb->hdr.bVersion = CUR_BLOB_VERSION;
  kb->hdr.reserved = 0;
  kb->hdr.aiKeyAlg = CALG_RC2;
  kb->dwKeySize = secret.size();
  memcpy(&kb->rgbKeyData, secret.c_str(), secret.size());

  HCRYPTPROV hProv = 0;
  HCRYPTKEY hKey = 0;
  HCRYPTHASH hHmacHash = 0;
  BYTE pbHash[32];
  DWORD dwDataLen = 32;
  HMAC_INFO HmacInfo;
  ZeroMemory(&HmacInfo, sizeof(HmacInfo));
  HmacInfo.HashAlgid = CALG_SHA1;

  CryptAcquireContext(&hProv, NULL,
    MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_NEWKEYSET);
  CryptImportKey(hProv, (BYTE*)kb, kbLen, 0, CRYPT_IPSEC_HMAC_KEY, &hKey);
  CryptCreateHash(hProv, CALG_HMAC, hKey, 0, &hHmacHash);
  CryptSetHashParam(hHmacHash, HP_HMAC_INFO, (BYTE*)&HmacInfo, 0);
  CryptHashData(hHmacHash, (BYTE*)(src.c_str()), src.size(), 0);
  CryptGetHashParam(hHmacHash, HP_HASHVAL, pbHash, &dwDataLen, 0);

  LocalFree(kb);
  CryptDestroyHash(hHmacHash);
  CryptDestroyKey(hKey);
  CryptReleaseContext(hProv, 0);

  DWORD dlen = 0;
  CryptBinaryToString(pbHash,
    dwDataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &dlen);
  char* dest = new char[dlen];
  CryptBinaryToString(pbHash,
    dwDataLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, dest, &dlen);

  std::string ret = std::string(dest, dlen);
  delete dest;
  return ret;
#else
  unsigned char md[EVP_MAX_BLOCK_LENGTH];
  unsigned int mdLen = EVP_MAX_BLOCK_LENGTH;

  if (HMAC(EVP_sha1(), secret.c_str(), secret.size(),
    reinterpret_cast<const unsigned char*>(stringToSign.c_str()), stringToSign.size(),
    md, &mdLen) == nullptr)
    return string();

  char encodedData[100];
  EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encodedData), md, mdLen);
  return encodedData;
#endif
}
