// This file is auto-generated, don't edit it. Thanks.
package com.aliyun.roautil;

import com.aliyun.tea.TeaRequest;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class ClientTest {

    @Test
    public void is4XXor5XXTest() throws Exception {
        new Client();
        Assert.assertFalse(Client.is4XXor5XX(200));
        Assert.assertFalse(Client.is4XXor5XX(888));
        Assert.assertTrue(Client.is4XXor5XX(503));
    }

    @Test
    public void getSignatureTest() throws Exception {
        TeaRequest request = new TeaRequest();
        String signature = Client.getStringToSign(request);
        Assert.assertEquals("GET\n\n\n\n\nnull", signature);
        Assert.assertEquals("RhNhDHvBaWVHInTaWtSYFAUQ3HU=", Client.getSignature(signature, "test"));

        Map<String, String> requestMap = new HashMap<>();
        requestMap.put("x-acs-security-token", "test");
        requestMap.put("x-acs-security-test", "test");
        requestMap.put("accept", "accept");
        requestMap.put("content-md5", "content-md5");
        requestMap.put("content-type", "content-type");
        requestMap.put("date", "date");
        requestMap.put("emptyTest", "");
        request.headers = requestMap;
        request.query = requestMap;
        request.pathname = "/test";
        signature = Client.getStringToSign(request);
        Assert.assertEquals("GET\n" +
                        "accept\n" +
                        "content-md5\n" +
                        "content-type\n" +
                        "date\n" +
                        "x-acs-security-test:test\n" +
                        "x-acs-security-token:test\n" +
                        "/test?accept=accept&content-md5=content-md5&content-type=content-type&date=date&emptyTest&x-acs-security-test=test&x-acs-security-token=test",
                signature);
        Assert.assertEquals("osUwxrPYOuYWu1I0k/biIeDczGw=", Client.getSignature(signature, "test"));
    }

    @Test
    public void deleteSpecialKeyTest() {
        Map<String, Object> map = new HashMap<>();
        map.put("test", "test");
        map.put("delete", "test");
        Map<String, Object> result = Client.deleteSpecialKey(map, "delete");
        Assert.assertNull(Client.deleteSpecialKey(null,null));
        Assert.assertNull(result.get("delete"));
        Assert.assertEquals("test", result.get("test"));
    }

    @Test
    public void queryTest() throws Exception {
        Map<String, Object> query = new HashMap<>();
        query.put("StringTest", "test");
        query.put("nullTest", null);
        query.put("IntegerTest", Integer.valueOf(1));

        List<Object> firstList = new ArrayList<>();
        firstList.add(Integer.valueOf(1));
        firstList.add(null);
        Map<String, Object> subMapInFirstList = new HashMap<>();
        subMapInFirstList.put("nullTest", null);
        subMapInFirstList.put("IntegerTest", Integer.valueOf(2));
        subMapInFirstList.put("StringTest", "test");
        firstList.add(subMapInFirstList);
        List<Object> secondList = new ArrayList<>();
        secondList.add(Integer.valueOf(1));
        secondList.add(null);
        firstList.add(secondList);
        query.put("list", firstList);


        Map<String, Object> firstMap = new HashMap<>();
        Map<String, Object> subMapInFirstMap = new HashMap<>();
        subMapInFirstMap.put("nullTest", null);
        subMapInFirstMap.put("IntegerTest", Integer.valueOf(2));
        subMapInFirstMap.put("StringTest", "test");
        subMapInFirstList.put("nullTest", null);
        subMapInFirstList.put("IntegerTest", Integer.valueOf(2));
        subMapInFirstList.put("StringTest", "test");
        firstMap.put("firstMapMap", subMapInFirstMap);
        firstMap.put("firstMapList", secondList);
        firstMap.put("nullTest", null);
        firstMap.put("IntegerTest", Integer.valueOf(2));
        firstMap.put("StringTest", "test");
        query.put("map", firstMap);

        Map<String, String> result = Client.query(null);
        Assert.assertEquals(0, result.size());

        result = Client.query(query);
        Assert.assertEquals("test", result.get("StringTest"));
        Assert.assertNull(result.get("nullTest"));
        Assert.assertEquals("1", result.get("IntegerTest"));

        Assert.assertEquals("1", result.get("list.1"));
        Assert.assertNull(result.get("list.2"));
        Assert.assertEquals("1", result.get("IntegerTest"));
        Assert.assertEquals("2", result.get("list.3.IntegerTest"));
        Assert.assertNull(result.get("list.3.nulTest"));
        Assert.assertEquals("test", result.get("list.3.StringTest"));
        Assert.assertEquals("1", result.get("list.4.1"));

        Assert.assertEquals("1", result.get("map.firstMapList.1"));
        Assert.assertNull(result.get("map.nullTest"));
        Assert.assertEquals("2", result.get("map.IntegerTest"));
        Assert.assertEquals("test", result.get("map.StringTest"));
        Assert.assertNull(result.get("map.firstMapMap.nullTest"));
        Assert.assertEquals("2", result.get("map.firstMapMap.IntegerTest"));
        Assert.assertEquals("test", result.get("map.firstMapMap.StringTest"));
    }

    @Test
    public void toFormStringTest() throws Exception {
        Map<String, Object> map = new HashMap<>();
        String result = Client.toForm(null);
        Assert.assertEquals("", result);

        result = Client.toForm(map);
        Assert.assertEquals("", result);

        map.put("form", "test");
        map.put("param", "test");
        map.put("nullTest", null);
        result = Client.toForm(map);
        Assert.assertEquals("form=test&param=test", result);
    }
}
