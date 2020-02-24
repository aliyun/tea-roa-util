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
        Assert.assertEquals("GET\n\n\n\n\n\nnull", signature);
        Assert.assertEquals("hD/QuFdv131xEuE0McZ/8dZi6ZQ=", Client.getSignature(signature, "test"));

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
                        "/test?date=datex-acs-security-token=testx-acs-security-test=testcontent-md5=content-md5content-type=content-typeaccept=accept",
                signature);
        Assert.assertEquals("Y69tKBaO0Z1WEKuDhYnUiltV23s=", Client.getSignature(signature, "test"));
    }
}
