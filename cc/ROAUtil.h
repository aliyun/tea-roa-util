#ifndef ROAUTIL_H_
#define ROAUTIL_H_

#include "Tea.h"

namespace ROAUtil
{

class Client
{
public:
    static bool is4XXor5XX(int code);
    static string getStringToSign(Tea::Request request);
    static string getSignature(string stringToSign, string accessKeySecret);
};

} // namespace ROAUtil
#endif