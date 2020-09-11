#include "hmac.hpp"
#include <alibabacloud/roautil.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <boost/functional/hash.hpp>
#include <darabonba/core.hpp>
#include <iostream>
#include <map>

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
    return (0 == fullString.compare(fullString.length() - ending.length(),
                                    ending.length(), ending));
  } else {
    return false;
  }
}

bool start_with(std::string const &fullString, std::string const &ending) {
  return fullString.rfind(ending, 0) == 0;
}

void replace_all(std::string &str, const std::string &from,
                 const std::string &to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing
                              // 'x' with 'yx'
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
  for (const auto &it : headers) {
    string key = it.first;
    if (start_with(key, "x-acs-")) {
      string val = filter(it.second);
      boost::trim(val);
      str.append(key).append(":").append(val).append("\n");
    }
  }
  return str;
}

string get_canonicalized_resource(string pathname,
                                  const map<string, string> &query) {
  if (query.empty()) {
    return pathname;
  }
  vector<string> tmp;
  for (auto it : query) {
    string str;
    if (it.second.empty()) {
      tmp.push_back(it.first);
    } else {
      tmp.push_back(str.append(it.first).append("=").append(it.second));
    }

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

  string result = implode(vector<string>{uppercase(request.method), accept,
                                         content_md5, content_type, date},
                          "\n").append("\n");

  string canonicalized_headers = get_canonicalized_headers(request.headers);
  string canonicalized_resource = get_canonicalized_resource(pathname, query);
  return result.append(canonicalized_headers).append(canonicalized_resource);
}

string Alibabacloud_ROAUtil::Client::getSignature(const string &stringToSign,
                                                  const string &secret) {
  return hmacSha1(stringToSign, secret);
}

void flatten(map<string, boost::any> &res, std::string prefix,
             boost::any curr) {
  if (typeid(map<string, boost::any>) == curr.type()) {
    map<string, boost::any> m = boost::any_cast<map<string, boost::any>>(curr);
    for (const auto &it : m) {
      std::string p;
      if (prefix.empty()) {
        p = prefix.append(it.first);
      } else {
        p = prefix.append(".").append(it.first);
      }
      flatten(res, p, it.second);
    }
  } else if (typeid(vector<boost::any>) == curr.type()) {
    vector<boost::any> v = boost::any_cast<vector<boost::any>>(curr);
    int n = 0;
    for (const auto &it : v) {
      std::string p;
      if (prefix.empty()) {
        p = prefix.append(to_string(n));
      } else {
        p = prefix.append(".").append(to_string(n));
      }
      flatten(res, p, it);
      n++;
    }
  } else {
    res.insert(pair<string, boost::any>(prefix, curr));
  }
}

string url_encode(const std::string &str) {
  std::stringstream escaped;
  escaped.fill('0');
  escaped << hex;

  for (char c : str) {
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }
    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char)c);
    escaped << nouppercase;
  }

  return escaped.str();
}

string Alibabacloud_ROAUtil::Client::toForm(map<string, boost::any> filter) {
  map<string, string> query;
  map<string, boost::any> flat;
  flatten(flat, string(""), boost::any(filter));
  for (const auto &it : flat) {
    boost::any val = it.second;
    if (typeid(string) == val.type()) {
      std::string v = boost::any_cast<string>(val);
      query[it.first] = v;
    } else if (typeid(int) == val.type()) {
      int v = boost::any_cast<int>(val);
      query[it.first] = std::to_string(v);
    } else if (typeid(long) == val.type()) {
      long v = boost::any_cast<long>(val);
      query[it.first] = std::to_string(v);
    } else if (typeid(double) == val.type()) {
      auto v = boost::any_cast<double>(val);
      query[it.first] = std::to_string(v);
    } else if (typeid(float) == val.type()) {
      auto v = boost::any_cast<float>(val);
      query[it.first] = std::to_string(v);
    } else if (typeid(bool) == val.type()) {
      auto b = boost::any_cast<bool>(val);
      string c = b ? "true" : "false";
      query[it.first] = c;
    } else if (typeid(const char *) == val.type()) {
      const char *v = boost::any_cast<const char *>(val);
      query[it.first] = v;
    } else if (typeid(char *) == val.type()) {
      char *v = boost::any_cast<char *>(val);
      query[it.first] = v;
    }
  }
  vector<string> v;
  for (const auto &it : query) {
    string str;
    v.push_back(str.append(it.first).append("=").append(url_encode(it.second)));
  }
  return implode(v, "&");
}

void Alibabacloud_ROAUtil::Client::convert(Model& body, Model& content) {
  map<std::string, boost::any> properties = body.toMap();
  for (const auto &it : properties) {
    content.set(it.first, it.second);
  }
}
