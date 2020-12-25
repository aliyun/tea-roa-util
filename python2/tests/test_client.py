import os
import unittest
from Tea.request import TeaRequest
from alibabacloud_roa_util.client import Client


class TestClient(unittest.TestCase):
    class TestConvertModel:
        def __init__(self):
            self.requestId = "test"
            self.dic = {}
            self.no_map = 1
            self.sub_model = None
            self.file = None

        def to_map(self):
            dic = {
                'requestId': self.requestId,
                'dic': self.dic,
                'no_map': self.no_map,
                'sub_model': self.sub_model,
                'file': self.file
            }
            return dic

    class TestConvertSubModel:
        def __init__(self):
            self.requestId = "subTest"
            self.id = 2

        def to_map(self):
            dic = {
                'requestId': self.requestId,
                'id': self.id
            }
            return dic

    class TestConvertMapModel:
        def __init__(self):
            self.requestId = ""
            self.extendId = 0
            self.dic = {}
            self.sub_model = None

        def to_map(self):
            dic = {
                'requestId': self.requestId,
                'dic': self.dic,
                'extendId': self.extendId,
                'sub_model': self.sub_model,
            }
            return dic

        def from_map(self, dic):
            self.requestId = dic.get("requestId") or ""
            self.extendId = dic.get("extendId") or 0
            self.dic = dic.get("dic")
            self.sub_model = dic.get("sub_model")

    def test_get_string_to_sign(self):
        request = TeaRequest()
        str_to_sign = Client.get_string_to_sign(request)
        self.assertEqual('GET\n\n\n\n\n', str_to_sign)

        request = TeaRequest()
        request.method = "POST"
        request.query = {
            'test': 'tests'
        }
        str_to_sign = Client.get_string_to_sign(request)
        self.assertEqual('POST\n\n\n\n\n?test=tests', str_to_sign)

        request = TeaRequest()
        request.headers = {
            'content-md5': 'md5',
        }
        str_to_sign = Client.get_string_to_sign(request)
        self.assertEqual('GET\n\nmd5\n\n\n', str_to_sign)

        request = TeaRequest()
        request.pathname = "Pathname"
        request.query = {
            'ccp': 'ok',
            'test': 'tests',
            'test1': ''
        }
        request.headers = {
            'x-acs-meta': 'user',
            "accept": "application/json",
            'content-md5': 'md5',
            'content-type': 'application/json',
            'date': 'date'
        }
        str_to_sign = Client.get_string_to_sign(request)
        s = 'GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-meta:user\nPathname?ccp=ok&test=tests&test1'
        self.assertEqual(s, str_to_sign)

    def test_get_signature(self):
        request = TeaRequest()
        str_to_sign = Client.get_string_to_sign(request)
        signature = Client.get_signature(str_to_sign, 'secret')
        self.assertEqual('GET\n\n\n\n\n', str_to_sign)
        self.assertEqual('XGXDWA78AEvx/wmfxKoVCq/afWw=', signature)

    def test_to_form(self):
        filter = {
            'client': 'test',
            'client1': None,
            'strs': ['str1', 'str2'],
            'tag': {
                'key': 'value'
            }
        }
        result = Client.to_form(filter)
        self.assertEqual('client=test&strs.1=str1&strs.2=str2&tag.key=value', result)

    def test_convert(self):
        module_path = os.path.dirname(__file__)
        filename = module_path + "/test.txt"
        with open(filename, 'rb') as f:
            model = TestClient.TestConvertModel()
            model.dic["key"] = "value"
            model.dic["testKey"] = "testValue"
            sub_model = TestClient.TestConvertSubModel()
            model.sub_model = sub_model
            model.file = f
            map_model = TestClient.TestConvertMapModel()
            Client.convert(model, map_model)
            self.assertIsNotNone(map_model)
            self.assertEqual("test", map_model.requestId)
            self.assertEqual(0, map_model.extendId)