using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

using Tea;
using Tea.Utils;

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
            string header = method + "\n" + accept + "\n" + contentMD5 + "\n" + contentType + "\n" + date + "\n";
            string canonicalizedHeaders = GetCanonicalizedHeaders(headers);
            string canonicalizedResource = GetCanonicalizedResource(pathname, query);
            string stringToSign = header + canonicalizedHeaders + canonicalizedResource;
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

        public static string ToForm(IDictionary filter)
        {
            if(filter == null)
            {
                return string.Empty;
            }
            Dictionary<string, object> dict = filter.Keys.Cast<string>().ToDictionary(key => key, key => filter[key]);
            Dictionary<string, string> outDict = new Dictionary<string, string>();
            TileDict(outDict, dict);
            List<string> listStr = new List<string>();
            foreach(var keypair in outDict)
            {
                if(string.IsNullOrWhiteSpace(keypair.Value))
                {
                    continue;
                }
                listStr.Add(PercentEncode(keypair.Key) + "=" + PercentEncode(keypair.Value));
            }
            return string.Join("&", listStr);
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
                
                String key = canonicalizedKeys[i];
                result.Append(key);
                result.Append(":");
                result.Append(headers[key].Trim());
                result.Append("\n");
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
            keys.Sort();
            string key;
            List<string> result = new List<string>();
            for (int i = 0; i < keys.Count; i++)
            {
                key = keys[i];
                if(string.IsNullOrWhiteSpace(query[key]))
                {
                    continue;
                }
                result.Add(key + "=" + query[key]);
            }
            return pathname + "?" + string.Join("&", result);
        }

        internal static void TileDict(Dictionary<string, string> dicOut, object obj, string parentKey = "")
        {
            if (obj == null)
            {
                return;
            }
            if (typeof(IDictionary).IsAssignableFrom(obj.GetType()))
            {
                Dictionary<string, object> dicIn = ((IDictionary)obj).Keys.Cast<string>().ToDictionary(key => key, key => ((IDictionary)obj)[key]);
                foreach (var keypair in dicIn)
                {
                    string keyName = parentKey + "." + keypair.Key;
                    if (keypair.Value == null)
                    {
                        continue;
                    }
                    TileDict(dicOut, keypair.Value, keyName);
                }
            }
            else if (typeof(IList).IsAssignableFrom(obj.GetType()))
            {
                int index = 1;
                foreach (var temp in (IList)obj)
                {
                    TileDict(dicOut, temp, parentKey + "." + index.ToSafeString());
                    index++;
                }
            }
            else
            {
                dicOut.Add(parentKey.TrimStart('.'), obj.ToSafeString(""));
            }
        }

        internal static string PercentEncode(string value)
        {
            if (value == null)
            {
                return null;
            }
            var stringBuilder = new StringBuilder();
            var text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~";
            var bytes = Encoding.UTF8.GetBytes(value);
            foreach (char c in bytes)
            {
                if (text.IndexOf(c) >= 0)
                {
                    stringBuilder.Append(c);
                }
                else
                {
                    stringBuilder.Append("%").Append(string.Format(CultureInfo.InvariantCulture, "{0:X2}", (int)c));
                }
            }

            return stringBuilder.ToString().Replace("+", "%20")
                .Replace("*", "%2A").Replace("~", "%7E");
        }
    }
}
