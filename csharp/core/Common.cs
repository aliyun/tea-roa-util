using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

using Tea;

namespace AlibabaCloud.ROAUtil
{
    public static class Common
    {
        public static bool Is4XXor5XX(int? code)
        {
            return code >= 400 && code < 600;
        }

        public static string GetStringToSign(TeaRequest request)
        {
            string method = request.Method;
            string pathname = request.Pathname;
            Dictionary<string, string> headers = request.Headers;
            Dictionary<string, string> query = request.Query;
            string accept = headers.ContainsKey("accept") ? headers["accept"] : "";
            string contentMD5 = headers.ContainsKey("content-md5") ? headers["content-md5"] : "";
            string contentType = headers.ContainsKey("content-type") ? headers["content-type"] : "";
            string date = headers.ContainsKey("date") ? headers["date"] : "";
            string header = method + "\n" + accept + "\n" + contentMD5 + "\n" + contentType + "\n" + date;
            string canonicalizedHeaders = GetCanonicalizedHeaders(headers);
            string canonicalizedResource = GetCanonicalizedResource(pathname, query);
            string stringToSign = header + "\n" + canonicalizedHeaders + "\n" + canonicalizedResource;
            return stringToSign;
        }

        public static string GetSignature(string stringToSign, string secret)
        {
            byte[] signData;
            using(KeyedHashAlgorithm algorithm = CryptoConfig.CreateFromName("HMACSHA1") as KeyedHashAlgorithm)
            {
                algorithm.Key = Encoding.UTF8.GetBytes(secret);
                signData = algorithm.ComputeHash(Encoding.UTF8.GetBytes(stringToSign.ToCharArray()));
            }
            return Convert.ToBase64String(signData);
        }

        public static Dictionary<string, object> DeleteSpecialKey(IDictionary obj, string key)
        {
            if (obj == null)
            {
                return null;
            }
            Dictionary<string, object> dictObj = obj.Keys.Cast<string>().ToDictionary(k => k, k => obj[k]);
            dictObj.Remove(key);
            return dictObj;
        }

        internal static string GetCanonicalizedHeaders(Dictionary<String, String> headers)
        {
            String prefix = "x-acs-";
            List<string> canonicalizedKeys = new List<string>();
            canonicalizedKeys = headers.Where(p => p.Key.StartsWith(prefix))
                .Select(p => p.Key).ToList();
            canonicalizedKeys.Sort();
            StringBuilder result = new StringBuilder();
            for (int i = 0; i < canonicalizedKeys.Count; i++)
            {
                if (i > 0)
                {
                    result.Append("\n");
                }
                String key = canonicalizedKeys[i];
                result.Append(key);
                result.Append(":");
                result.Append(headers[key].Trim());
            }
            return result.ToString();
        }

        internal static string GetCanonicalizedResource(String pathname, Dictionary<String, String> query)
        {
            if (query == null || query.Count <= 0)
            {
                return pathname;
            }
            List<string> keys = query.Keys.ToList();
            StringBuilder result = new StringBuilder(pathname);
            result.Append("?");
            String key;
            for (int i = 0; i < keys.Count; i++)
            {
                key = keys[i];
                result.Append(key);
                result.Append("=");
                result.Append(query[key]);
            }
            return result.ToString();
        }
    }
}
