// This file is auto-generated, don't edit it. Thanks.

#ifndef ALIBABACLOUD_ROAUTIL_H_
#define ALIBABACLOUD_ROAUTIL_H_

#include <boost/any.hpp>
#include <darabonba/core.hpp>
#include <iostream>
#include <map>

using namespace std;

namespace Alibabacloud_ROAUtil {
class Client {
public:
  static string getStringToSign(shared_ptr<Darabonba::Request> request);
  static string getSignature(shared_ptr<string> stringToSign, shared_ptr<string> secret);
  static string toForm(shared_ptr<map<string, boost::any>> filter);
  static void convert(shared_ptr<Darabonba::Model> body, shared_ptr<Darabonba::Model> content);

  Client() {};
  ~Client() {};
};
} // namespace Alibabacloud_ROAUtil

#endif
