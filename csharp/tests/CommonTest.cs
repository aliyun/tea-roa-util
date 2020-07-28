using System.Collections.Generic;

using AlibabaCloud.ROAUtil;

using Tea;
using tests.Models;
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
            query.Add("emptyKey", "");
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

        [Fact]
        public void Test_ToForm()
        {
            Assert.Empty(Common.ToForm(null));

            Dictionary<string, object> dict = new Dictionary<string, object>();
            Assert.Empty(Common.ToForm(dict));

            Dictionary<string, object> dicObj = new Dictionary<string, object>();
            dicObj.Add("test", "test");
            dicObj.Add("key", "value");
            dicObj.Add("null", null);
            Dictionary<string, object> subDict = new Dictionary<string, object>();
            subDict.Add("subKey", "subValue");
            subDict.Add("subTest", "subTest");
            subDict.Add("subListInt", new List<int> { 1, 2, 3 });
            subDict.Add("subNull", null);
            subDict.Add("subListDict", new List<Dictionary<string, object>>
            {
                new Dictionary<string, object> { { "a", "b" }, { "c", "d" } },
                new Dictionary<string, object> { { "e", "f" }, { "g", "h" } },
            });
            dicObj.Add("sub", subDict);
            List<object> listObj = new List<object>
            {
                new Dictionary<string, object> { { "a", "b" }, { "c", "d" } },
                5,
                new List<string> { "list1", "list2" }
            };

            dicObj.Add("slice", listObj);
            string result = Common.ToForm(dicObj);
            Assert.Equal("test=test&key=value&sub.subKey=subValue&sub.subTest=subTest&sub.subListInt.1=1&sub.subListInt.2=2&sub.subListInt.3=3&sub.subListDict.1.a=b&sub.subListDict.1.c=d&sub.subListDict.2.e=f&sub.subListDict.2.g=h&slice.1.a=b&slice.1.c=d&slice.2=5&slice.3.1=list1&slice.3.2=list2", result);

        }

        [Fact]
        public void Test_Convert()
        {
            TestConvertModel model = new TestConvertModel
            {
                RequestId = "test",
                Dict = new Dictionary<string, object>
                { { "key", "value" },
                { "testKey", "testValue" }
                },
                NoMap = 1,
                SubModel = new TestConvertModel.TestConvertSubModel
                {
                    Id = 2,
                    RequestId = "subTest"
                }
            };

            TestConvertMapModel mapModel = new TestConvertMapModel();
            Common.Convert(model, mapModel);
            Assert.Equal("test", mapModel.RequestId);
            Assert.Equal(0, mapModel.ExtendId);
            Assert.Equal(2, mapModel.SubModel.Id);
        }
    }
}
