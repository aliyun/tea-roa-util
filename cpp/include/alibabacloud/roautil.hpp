#ifndef ALIBABACLOUD_ROAUTIL_H_
#define ALIBABACLOUD_ROAUTIL_H_

#include <boost/any.hpp>
#include <darabonba/core.hpp>
#include <iostream>
#include <map>

using namespace Darabonba;
using namespace std;

namespace Alibabacloud_ROAUtil {
class Client : public Model {
public:
  Client();
  ~Client();

  static string getStringToSign(Request *request);
  static string getSignature(string *stringToSign, string *secret);
  static string toForm(map<string, boost::any> *filter);
  static void convert(Model *body, Model *content);
};
} // namespace Alibabacloud_ROAUtil

#endif
