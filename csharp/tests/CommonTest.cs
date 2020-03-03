using System.Collections.Generic;

using AlibabaCloud.ROAUtil;

using Tea;

using Xunit;

namespace tests
{
    public class CommonTest
    {
        [Fact]
        public void Test_Is4XXor5XX()
        {
            Assert.True(Common.Is4XXor5XX(400));
            Assert.False(Common.Is4XXor5XX(200));
            Assert.False(Common.Is4XXor5XX(800));
        }

        [Fact]
        public void TestGetSignature()
        {
            TeaRequest teaRequestEmpty = new TeaRequest();
            teaRequestEmpty.Method = "GET";
            teaRequestEmpty.Pathname = "Pathname";
            Dictionary<string, string> headersEmpty = new Dictionary<string, string>();
            teaRequestEmpty.Headers = headersEmpty;
            Dictionary<string, string> querysEmpty = new Dictionary<string, string>();
            teaRequestEmpty.Query = querysEmpty;
            Assert.NotNull(Common.GetStringToSign(teaRequestEmpty));
            Assert.NotNull(Common.GetSignature(Common.GetStringToSign(teaRequestEmpty), "accessKeySecret"));

            TeaRequest teaRequest = new TeaRequest();
            teaRequest.Method = "GET";
            teaRequest.Pathname = "Pathname";
            Dictionary<string, string> headers = new Dictionary<string, string>();
            headers.Add("headerKey", "headerValue");
            headers.Add("accept", "accept");
            headers.Add("content-md5", "content-md5");
            headers.Add("content-type", "content-type");
            headers.Add("date", "date");
            teaRequest.Headers = headers;
            Dictionary<string, string> querys = new Dictionary<string, string>();
            querys.Add("queryKey", "queryValue");
            teaRequest.Query = querys;
            Assert.NotNull(Common.GetStringToSign(teaRequest));
            Assert.NotNull(Common.GetSignature(Common.GetStringToSign(teaRequest), "accessKeySecret"));
        }

        [Fact]
        public void Test_DeleteSpecialKey()
        {
            Assert.Null(Common.DeleteSpecialKey(null, null));

            Dictionary<string, object> dict = new Dictionary<string, object>();
            dict.Add("test", "testVal");
            dict.Add("delete", "deleteValue");
            Dictionary<string, object> result = Common.DeleteSpecialKey(dict, "delete");
            Assert.NotNull(result);
            Assert.False(result.ContainsKey("delete"));
            Assert.True(result.ContainsKey("test"));
        }
    }
}
