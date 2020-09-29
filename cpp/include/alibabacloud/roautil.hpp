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

  static string getStringToSign(shared_ptr<Request> request);
  static string getSignature(shared_ptr<string> stringToSign,
                             shared_ptr<string> secret);
  static string toForm(shared_ptr<map<string, boost::any>> filter);
  static void convert(shared_ptr<Model> body, shared_ptr<Model> content);
};
} // namespace Alibabacloud_ROAUtil

#endif
