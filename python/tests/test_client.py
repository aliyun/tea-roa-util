import unittest
from Tea.request import TeaRequest
from alibabacloud_roa_util.client import Client


class TestClient(unittest.TestCase):
    def test_get_str_to_sign(self):
        request = TeaRequest()
        str_to_sign = Client.get_str_to_sign(request)
        self.assertEqual('GET\n\n\n\n\n', str_to_sign)

        request = TeaRequest()
        request.method = "POST"
        request.query = {
            'test': 'tests'
        }
        str_to_sign = Client.get_str_to_sign(request)
        self.assertEqual('POST\n\n\n\n\n?test=tests', str_to_sign)

        request = TeaRequest()
        request.headers = {
            'content-md5': 'md5',
        }
        str_to_sign = Client.get_str_to_sign(request)
        self.assertEqual('GET\n\nmd5\n\n\n', str_to_sign)

        request = TeaRequest()
        request.pathname = "Pathname"
        request.query = {
            'ccp': 'ok',
            'test': 'tests'
        }
        request.headers = {
            'x-acs-meta': 'user',
            "accept": "application/json",
            'content-md5': 'md5',
            'content-type': 'application/json',
            'date': 'date'
        }
        str_to_sign = Client.get_str_to_sign(request)
        s = 'GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-meta:user\n?ccp=ok&test=tests'
        self.assertEqual(s, str_to_sign)

    def test_get_signature(self):
        request = TeaRequest()
        str_to_sign = Client.get_str_to_sign(request)
        signature = Client.get_signature(str_to_sign, 'secret')
        self.assertEqual('GET\n\n\n\n\n', str_to_sign)
        self.assertEqual('XGXDWA78AEvx/wmfxKoVCq/afWw=', signature)

