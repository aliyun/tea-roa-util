#include "gtest/gtest.h"
#include <alibabacloud/roautil.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace Darabonba;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class TestModel : public Model {
public:
  string getName() {
    return name;
  }

  string getTest() {
    return test;
  }

  void setName(string n) {
    name = std::move(n);
  }

  void setTest(string t) {
    test = std::move(t);
  }

  void validate() override {
    cout << "test validate";
  }

  map<string, boost::any> toMap() override {
    map<string, boost::any> result;
    result["name"] = name;
    result["test"] = test;
    return result;
  };

  void fromMap(map<string, boost::any> m) override {
    name = boost::any_cast<string>(m.at("name"));
    test = boost::any_cast<string>(m.at("test"));
  }
private:
  string name;
  string test;
};

TEST(tests, getSignature) {

  string res = Alibabacloud_ROAUtil::Client::getSignature(
      make_shared<string>(string("stringtosign")),
      make_shared<string>(string("secret"))
  );
  ASSERT_EQ(string("OmuTAr79tpI6CRoAdmzKRq5lHs0="), res);
}

TEST(tests, getStringToSign) {
  Request request;
  string str_to_sign = Alibabacloud_ROAUtil::Client::getStringToSign(make_shared<Request>(request));
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
  string res = Alibabacloud_ROAUtil::Client::getStringToSign(make_shared<Request>(request));
  string s = "GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-meta:user\nPathname?ccp=ok&test=tests&test1";
  ASSERT_EQ(s, res);
}

TEST(tests, toForm) {
  vector<boost::any> lis = {"str1", "str2"};
  map<string, boost::any> d = {
      {"key", "value"}
  };
  map<string, boost::any> m = {
      {"client", "test"},
      {"strs", lis},
      {"tag", d}
  };
  string result = Alibabacloud_ROAUtil::Client::toForm(make_shared<map<string, boost::any>>(m));
  ASSERT_EQ("client=test&strs.1=str1&strs.2=str2&tag.key=value", result);
}

TEST(tests, convert) {
  string name = "name";
  string test = "test";
  shared_ptr<TestModel> iModel(new TestModel);
  shared_ptr<TestModel> oModel(new TestModel);
  iModel->setName(name);
  iModel->setTest(test);
  Alibabacloud_ROAUtil::Client::convert(iModel, oModel);
  ASSERT_EQ(name, oModel->getName());
  ASSERT_EQ(test, oModel->getTest());
}
