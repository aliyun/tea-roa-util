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
        public void Test_GetStringToSign()
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
            teaRequest.Pathname = "/";
            Dictionary<string, string> headers = new Dictionary<string, string>();
            headers.Add("accept", "application/json");
            teaRequest.Headers = headers;
            Assert.Equal("GET\napplication/json\n\n\n\n/", Common.GetStringToSign(teaRequest));

            teaRequest.Headers.Add("content-md5", "md5");
            teaRequest.Headers.Add("content-type", "application/json");
            teaRequest.Headers.Add("date", "date");
            Assert.Equal("GET\napplication/json\nmd5\napplication/json\ndate\n/", Common.GetStringToSign(teaRequest));

            teaRequest.Headers.Add("x-acs-custom-key", "any value");
            Assert.Equal("GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/", Common.GetStringToSign(teaRequest));

            Dictionary<string, string> query = new Dictionary<string, string>();
            query.Add("key", "val ue with space");
            teaRequest.Query = query;
            Assert.Equal("GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/?key=val ue with space", Common.GetStringToSign(teaRequest));
        }

        [Fact]
        public void Test_GetSignature()
        {
            Assert.Equal("OmuTAr79tpI6CRoAdmzKRq5lHs0=", Common.GetSignature("stringtosign", "secret"));
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
