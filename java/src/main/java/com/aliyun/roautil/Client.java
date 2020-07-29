// This file is auto-generated, don't edit it. Thanks.
package com.aliyun.roautil;

import com.aliyun.tea.TeaModel;
import com.aliyun.tea.TeaRequest;
import com.aliyun.tea.utils.StringUtils;
import com.google.gson.Gson;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.net.URLEncoder;
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
        Arrays.sort(keys);
        StringBuilder result = new StringBuilder(pathname);
        result.append("?");
        String key;
        String value;
        for (int i = 0; i < keys.length; i++) {
            key = keys[i];
            result.append(key);
            value = query.get(key);
            if (!StringUtils.isEmpty(value) && !"".equals(value.trim())) {
                result.append("=");
                result.append(value);
            }
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


    /**
     * Parse filter into a form string
     * @param filter object
     * @return the string
     */
    public static String toForm(Map<String, ?> filter) throws Exception{
        Map<String, String> map = query(filter);
        StringBuilder result = new StringBuilder();
        boolean first = true;
        for (Map.Entry<String, ?> entry : map.entrySet()) {
            if (StringUtils.isEmpty(entry.getValue())) {
                continue;
            }
            if (first) {
                first = false;
            } else {
                result.append("&");
            }
            result.append(URLEncoder.encode(entry.getKey(), "UTF-8"));
            result.append("=");
            result.append(URLEncoder.encode(String.valueOf(entry.getValue()), "UTF-8"));
        }
        return result.toString();
    }

    public static Map<String, String> query(Map<String, ?> map) {
        Map<String, String> outMap = new HashMap<>();
        if (null != map) {
            processeObject(outMap, "", map);
        }
        return outMap;
    }


    private static void processeObject(Map<String, String> map, String key, Object value) {
        if (StringUtils.isEmpty(value)) {
            return;
        }
        if (value instanceof List) {
            List list = (List) value;
            for (int i = 0; i < list.size(); i++) {
                processeObject(map, key + "." + (i + 1), list.get(i));
            }
        } else if (value instanceof Map) {
            Map<String, Object> subMap = (Map<String, Object>) value;
            for (Map.Entry<String, Object> entry : subMap.entrySet()) {
                processeObject(map, key + "." + (entry.getKey()), entry.getValue());
            }
        } else {
            if (key.startsWith(".")) {
                key = key.substring(1);
            }
            map.put(key, String.valueOf(value));
        }
    }

    public static void convert(TeaModel source, TeaModel target) throws IllegalAccessException, InstantiationException {
        if (source == null || target == null) {
            return;
        }
        Class sourceClass = source.getClass();
        Class targetClass = target.getClass();
        Field[] fields = sourceClass.getDeclaredFields();
        TeaModel teaModel = (TeaModel) sourceClass.newInstance();
        for (Field field : fields) {
            field.setAccessible(true);
            if (InputStream.class.isAssignableFrom(field.getType())) {
                continue;
            }
            field.set(teaModel, field.get(source));
        }
        Gson gson = new Gson();
        String jsonString = gson.toJson(teaModel);
        Object outPut = gson.fromJson(jsonString, targetClass);
        fields = outPut.getClass().getFields();
        for (Field field : fields) {
            field.setAccessible(true);
            field.set(target, field.get(outPut));
        }
    }
}
