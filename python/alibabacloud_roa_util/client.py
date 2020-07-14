import hashlib
import hmac
import base64

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
            if query[key] == '':
                s = '%s&' % key
            else:
                s = '%s=%s&' % (key, query[key])
            resource += s
        return resource[:-1]

    @staticmethod
    def get_string_to_sign(request):
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
        hash_val = hmac.new(secret.encode('utf-8'), sign.encode('utf-8'), hashlib.sha1).digest()
        signature = base64.encodebytes(hash_val).decode('utf-8')
        return signature.rstrip('\n')

    @staticmethod
    def to_form(filter):
        result = {}
        if filter:
            Client._object_handler('', filter, result)
        return Util.to_form_string(
            Util.anyify_map_value(result)
        )

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
