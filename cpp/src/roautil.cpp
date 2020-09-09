#include <alibabacloud/roautil.hpp>
#include <boost/any.hpp>
#include <boost/exception/all.hpp>
#include <darabonba/core.hpp>
#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>
#include "hmac.h"
#include "sha1.h"

using namespace Darabonba;
using namespace std;

string find_header(map<string, string> headers, const string &header_name) {
  if (headers.find(header_name) != headers.end()) {
    return headers.at(header_name);
  }
  return string("");
}
std::string implode(const std::vector<std::string> &vec,
                    const std::string &glue) {
  string res;
  int n = 0;
  for (const auto &str : vec) {
    if (n == 0) {
      res = str;
    } else {
      res += glue + str;
    }
    n++;
  }
  return res;
}
string uppercase(string str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return str;
}

bool end_with(std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

bool start_with(std::string const &fullString, std::string const &ending) {
  return fullString.rfind("ending", 0) == 0;
}

void replace_all(std::string &str, const std::string &from, const std::string &to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}
string filter(string v) {
  replace_all(v, "\t", "");
  replace_all(v, "\n", "");
  replace_all(v, "\r", "");
  replace_all(v, "\f", "");
  return v;
}

string get_canonicalized_headers(const map<string, string> &headers) {
  string str;
  for (const auto &it:headers) {
    string key = it.first;
    if (start_with(key, "x-acs-")) {
      string val = filter(it.second);
      boost::trim(val);
      str.append(key).append(":").append(val).append("\n");
    }
  }
  return str;
}

string get_canonicalized_resource(string pathname, const map<string, string>& query) {
  if (query.empty()) {
    return pathname;
  }
  vector<string> tmp;
  for (auto it:query) {
    string str;
    tmp.push_back(str.append(it.first).append("=").append(it.second));
  }
  string res;
  return res.append(pathname).append("?").append(implode(tmp, "&"));
}

string Alibabacloud_ROAUtil::Client::getStringToSign(const Request &request) {
  string pathname = request.pathname;
  map<string, string> query = request.query;
  string accept = find_header(request.headers, "accept");
  string content_md5 = find_header(request.headers, "content-md5");
  string content_type = find_header(request.headers, "content-type");
  string date = find_header(request.headers, "date");

  string result = implode(vector<string>{
      uppercase(request.method),
      accept,
      content_md5,
      content_type,
      date
  }, "\n");

  string canonicalized_headers = get_canonicalized_headers(request.headers);
  string canonicalized_resource = get_canonicalized_resource(pathname, query);
  return result.append(canonicalized_headers).append(canonicalized_resource);
}

string Alibabacloud_ROAUtil::Client::getSignature(string stringToSign, string secret) {
//  return hmac<sha1>::calc_hex(stringToSign, secret);
}

string Alibabacloud_ROAUtil::Client::toForm(map<string, boost::any> filter) {
}

void Alibabacloud_ROAUtil::Client::convert(Model body, Model content) {
}

