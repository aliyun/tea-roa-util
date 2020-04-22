// This file is auto-generated, don't edit it. Thanks.
package com.aliyun.roautil;

import com.aliyun.tea.TeaRequest;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.util.*;


public class Client {

    public static boolean is4XXor5XX(Integer code) throws Exception {
        return code >= 400 && code < 600;
    }

    public static String getStringToSign(TeaRequest request) throws Exception {
        String method = request.method;
        String pathname = request.pathname;
        Map<String, String> headers = request.headers;
        Map<String, String> query = request.query;
        String accept = headers.get("accept") == null ? "" : headers.get("accept");
        String contentMD5 = headers.get("content-md5") == null ? "" : headers.get("content-md5");
        String contentType = headers.get("content-type") == null ? "" : headers.get("content-type");
        String date = headers.get("date") == null ? "" : headers.get("date");
        String header = method + "\n" + accept + "\n" + contentMD5 + "\n" + contentType + "\n" + date + "\n";
        String canonicalizedHeaders = getCanonicalizedHeaders(headers);
        String canonicalizedResource = getCanonicalizedResource(pathname, query);
        String stringToSign = header + canonicalizedHeaders + canonicalizedResource;
        return stringToSign;
    }

    protected static String getCanonicalizedHeaders(Map<String, String> headers) {
        String prefix = "x-acs-";
        Set<String> keys = headers.keySet();
        List<String> canonicalizedKeys = new ArrayList<>();
        for (String key : keys) {
            if (key.startsWith(prefix)) {
                canonicalizedKeys.add(key);
            }
        }
        String[] canonicalizedKeysArray = canonicalizedKeys.toArray(new String[canonicalizedKeys.size()]);
        Arrays.sort(canonicalizedKeysArray);
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < canonicalizedKeysArray.length; i++) {
            String key = canonicalizedKeysArray[i];
            result.append(key);
            result.append(":");
            result.append(headers.get(key).trim());
            result.append("\n");
        }
        return result.toString();
    }

    protected static String getCanonicalizedResource(String pathname, Map<String, String> query) {
        String[] keys = query.keySet().toArray(new String[query.size()]);
        if (keys.length <= 0) {
            return pathname;
        }
        StringBuilder result = new StringBuilder(pathname);
        result.append("?");
        String key;
        for (int i = 0; i < keys.length; i++) {
            key = keys[i];
            result.append(key);
            result.append("=");
            result.append(query.get(key));
            result.append("&");
        }
        return result.deleteCharAt(result.length() - 1).toString();
    }

    public static String getSignature(String stringToSign, String secret) throws Exception {
        Mac mac = Mac.getInstance("HmacSHA1");
        mac.init(new SecretKeySpec(secret.getBytes("UTF-8"), "HmacSHA1"));
        byte[] signData = mac.doFinal(stringToSign.getBytes("UTF-8"));
        return Base64.getEncoder().encodeToString(signData);
    }

    public static Map<String, Object> deleteSpecialKey(Map<String, ?> map, String key) {
        if (null == map) {
            return null;
        }
        HashMap<String, Object> result = new HashMap<>();
        result.putAll(map);
        result.remove(key);
        return result;
    }
}
