#include <iostream>

#ifdef _WIN32
#include <wincrypt.h>
#include <windows.h>
#else
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <list>
#include <openssl/hmac.h>
#endif

using namespace std;

string hmacSha1(const std::string &src, const std::string &secret) {
  if (src.empty())
    return std::string();

#ifdef _WIN32
  typedef struct _my_blob {
    BLOBHEADER hdr;
    DWORD dwKeySize;
    BYTE rgbKeyData[];
  } my_blob;

  DWORD kbLen = sizeof(my_blob) + secret.size();
  my_blob *kb = (my_blob *)LocalAlloc(LPTR, kbLen);
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

  CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL,
                      CRYPT_VERIFYCONTEXT | CRYPT_NEWKEYSET);
  CryptImportKey(hProv, (BYTE *)kb, kbLen, 0, CRYPT_IPSEC_HMAC_KEY, &hKey);
  CryptCreateHash(hProv, CALG_HMAC, hKey, 0, &hHmacHash);
  CryptSetHashParam(hHmacHash, HP_HMAC_INFO, (BYTE *)&HmacInfo, 0);
  CryptHashData(hHmacHash, (BYTE *)(src.c_str()), src.size(), 0);
  CryptGetHashParam(hHmacHash, HP_HASHVAL, pbHash, &dwDataLen, 0);

  LocalFree(kb);
  CryptDestroyHash(hHmacHash);
  CryptDestroyKey(hKey);
  CryptReleaseContext(hProv, 0);

  DWORD dlen = 0;
  CryptBinaryToString(pbHash, dwDataLen,
                      CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &dlen);
  char *dest = new char[dlen];
  CryptBinaryToString(pbHash, dwDataLen,
                      CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, dest, &dlen);

  std::string ret = std::string(dest, dlen);
  delete dest;
  return ret;
#else
  unsigned char md[EVP_MAX_BLOCK_LENGTH];
  unsigned int mdLen = EVP_MAX_BLOCK_LENGTH;

  if (HMAC(EVP_sha1(), secret.c_str(), secret.size(),
           reinterpret_cast<const unsigned char *>(src.c_str()), src.size(), md,
           &mdLen) == nullptr)
    return std::string();

  char encodedData[100];
  EVP_EncodeBlock(reinterpret_cast<unsigned char *>(encodedData), md, mdLen);
  return encodedData;
#endif
}
