import unittest
from Tea.request import TeaRequest
from roa_util import client


class TestClient(unittest.TestCase):
    def test_get_str_to_sign(self):
        request = TeaRequest()
        request.query = {
            'ccp': 'ok'
        }
        request.headers = {
            'x-acs-meta': 'user'
        }
        str_to_sign = client.get_str_to_sign(request)
        s = 'GET\n\n\n\n\nx-acs-meta:user\n?ccp=ok'
        self.assertEqual(s, str_to_sign)

    def test_get_signature(self):
        request = TeaRequest()
        str_to_sign = client.get_str_to_sign(request)
        signature = client.get_signature(str_to_sign, 'secret')
        self.assertEqual('GET\n\n\n\n\n', str_to_sign)
        self.assertEqual('XGXDWA78AEvx/wmfxKoVCq/afWw=', signature)

