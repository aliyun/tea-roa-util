#include "gtest/gtest.h"
#include <alibabacloud/roautil.hpp>

using namespace std;
using namespace Darabonba;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class TestModel: public Model
{
public:
  void validate() override {
    cout << "test validate";
  }
};

TEST(tests, getSignature) {
  string res = Alibabacloud_ROAUtil::Client::getSignature("stringtosign", "secret");
  ASSERT_EQ(string("OmuTAr79tpI6CRoAdmzKRq5lHs0="), res);
}

TEST(tests, getStringToSign)
{
  Request request;
  string str_to_sign = Alibabacloud_ROAUtil::Client::getStringToSign(request);
  ASSERT_EQ("GET\n\n\n\n\n", str_to_sign);
  request.pathname = "Pathname";
  map<string, string> query = {
      {"ccp", "ok"},
      {"test", "tests"},
      {"test1", ""}
  };
  map<string, string> headers = {
      {"x-acs-meta", "user"},
      {"accept", "application/json"},
      {"content-md5", "md5"},
      {"content-type", "application/json"},
      {"date", "date"}
  };
  request.query = query;
  request.headers = headers;
  string res = Alibabacloud_ROAUtil::Client::getStringToSign(request);
  string s = "GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-meta:user\nPathname?ccp=ok&test=tests&test1";
  ASSERT_EQ(s, res);
}

TEST(tests, toForm)
{
  vector<boost::any> lis = {"str1", "str2"};
  map<string, boost::any> d = {
      {"key", "value"}
  };
  const map<string, boost::any> m = {
      {"client", "test"},
      {"strs", lis},
      {"tag", d}
  };
  string result = Alibabacloud_ROAUtil::Client::toForm(m);
  ASSERT_EQ("client=test&strs.1=str1&strs.2=str2&tag.key=value", result);
}

TEST(tests, convert)
{
  TestModel iModel;
  TestModel oModel;
  string name = "name";
  string test = "test";
  iModel.set("name", name);
  iModel.set("test", test);
  Alibabacloud_ROAUtil::Client::convert(iModel, oModel);
  ASSERT_EQ(name, boost::any_cast<string>(oModel.get("name")));
  ASSERT_EQ(test, boost::any_cast<string>(oModel.get("test")));
}
