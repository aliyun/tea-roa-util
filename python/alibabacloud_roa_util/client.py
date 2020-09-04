import hashlib
import hmac
import base64
import copy

from Tea.stream import READABLE
from alibabacloud_tea_util.client import Client as Util


class Client:
    @staticmethod
    def _get_canonicalized_headers(headers):
        canon_keys = []
        for k in headers:
            if k.startswith('x-acs-'):
                canon_keys.append(k)
        canon_keys = sorted(canon_keys)
        canon_header = ''
        for k in canon_keys:
            canon_header += '%s:%s\n' % (k, headers[k])
        return canon_header

    @staticmethod
    def _get_canonicalized_resource(pathname, query):
        if len(query) <= 0:
            return pathname
        resource = '%s?' % pathname
        query_list = sorted(list(query))
        for key in query_list:
            if query[key] is not None:
                if query[key] == '':
                    s = '%s&' % key
                else:
                    s = '%s=%s&' % (key, query[key])
                resource += s
        return resource[:-1]

    @staticmethod
    def get_string_to_sign(request):
        """
        Get the string to be signed according to request
        @param request  which contains signed messages
        @return the signed string
        """
        method, pathname, headers, query = request.method, request.pathname, request.headers, request.query

        accept = '' if headers.get('accept') is None else headers.get('accept')
        content_md5 = '' if headers.get('content-md5') is None else headers.get('content-md5')
        content_type = '' if headers.get('content-type') is None else headers.get('content-type')
        date = '' if headers.get('date') is None else headers.get('date')

        header = '%s\n%s\n%s\n%s\n%s\n' % (method, accept, content_md5, content_type, date)
        canon_headers = Client._get_canonicalized_headers(headers)
        canon_resource = Client._get_canonicalized_resource(pathname, query)
        sign_str = header + canon_headers + canon_resource
        return sign_str

    @staticmethod
    def get_signature(sign, secret):
        """
        Get signature according to stringToSign, secret
        @param sign the signed string
        @param secret accesskey secret
        @return the signature
        """
        hash_val = hmac.new(secret.encode('utf-8'), sign.encode('utf-8'), hashlib.sha1).digest()
        signature = base64.b64encode(hash_val).decode('utf-8')
        return signature

    @staticmethod
    def to_form(filter):
        """
        Parse filter into a form string
        @param filter object
        @return the string
        """
        result = {}
        if filter:
            Client._object_handler('', filter, result)
        return Util.to_form_string(
            Util.anyify_map_value(result)
        )

    @staticmethod
    def convert(body, content):
        """
        Convert all params of body other than type of readable into content
        @param body source Model
        @param content target Model
        @return void
        """
        pros = {}
        body_map = body.to_map()
        for k, v in body_map.items():
            if not isinstance(v, READABLE):
                pros[k] = copy.deepcopy(v)

        content.from_map(pros)

    @staticmethod
    def _object_handler(key, value, out):
        if value is None:
            return

        if isinstance(value, dict):
            dic = value
            for k, v in dic.items():
                Client._object_handler('%s.%s' % (key, k), v, out)
        elif isinstance(value, (list, tuple)):
            lis = value
            for index, val in enumerate(lis):
                Client._object_handler('%s.%s' % (key, index + 1), val, out)
        else:
            if key.startswith('.'):
                key = key[1:]
            out[key] = str(value)
