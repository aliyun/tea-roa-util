// This file is auto-generated, don't edit it. Thanks.
package com.aliyun.roautil;

import com.aliyun.tea.TeaRequest;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;
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
        request.headers = requestMap;
        request.query = requestMap;
        request.pathname = "/test";
        signature = Client.getStringToSign(request);
        Assert.assertEquals("GET\naccept\ncontent-md5\ncontent-type\ndate\nx-acs-security-test:test\nx-acs-security-token:test\n" +
                        "/test?date=date&x-acs-security-token=test&x-acs-security-test=test&content-md5=content-md5&content-type=content-type&accept=accept",
                signature);
        Assert.assertEquals("SsWK5BHjFUe3pquMl4/gGJYhNzU=", Client.getSignature(signature, "test"));
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
}
