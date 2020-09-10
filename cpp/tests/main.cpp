#include "gtest/gtest.h"
#include <alibabacloud/roautil.hpp>

using namespace std;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(tests, getSignature) {
  string res = Alibabacloud_ROAUtil::Client::getSignature("stringtosign", "secret");
  ASSERT_EQ(string("OmuTAr79tpI6CRoAdmzKRq5lHs0="), res);
}