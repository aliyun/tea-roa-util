import Client from '../src/client';

import { Readable } from 'stream';
import * as $tea from '@alicloud/tea-typescript';
import assert from 'assert';
import 'mocha';

describe('Tea Util', function () {
    it('Module should ok', function () {
        assert.ok(Client);
    });

    it('is4XXor5XX', function () {
        assert.deepStrictEqual(Client.is4XXor5XX(300), false);
        assert.deepStrictEqual(Client.is4XXor5XX(600), false);
        assert.deepStrictEqual(Client.is4XXor5XX(404), true);
        assert.deepStrictEqual(Client.is4XXor5XX(500), true);
    });

    it('getSignature', function () {
        assert.deepStrictEqual(Client.getSignature('stringtosign', 'secret'), 'OmuTAr79tpI6CRoAdmzKRq5lHs0=');
    });

    it('getStringToSign', function () {
        const request = new $tea.Request();
        request.method = 'GET';
        request.pathname = '/';
        request.headers = {
            'accept': 'application/json'
        };
        assert.deepStrictEqual(Client.getStringToSign(request), 'GET\napplication/json\n\n\n\n/');

        request.headers = {
            'accept': 'application/json',
            'content-md5': 'md5',
            'content-type': 'application/json',
            'date': 'date'
        };

        assert.deepStrictEqual(Client.getStringToSign(request), 'GET\napplication/json\nmd5\napplication/json\ndate\n/');

        request.headers = {
            'accept': 'application/json',
            'content-md5': 'md5',
            'content-type': 'application/json',
            'date': 'date',
            'x-acs-custom-key': 'any value'
        };

        assert.deepStrictEqual(Client.getStringToSign(request), 'GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/');

        request.query = {
            'key': 'val ue with space'
        };

        assert.deepStrictEqual(Client.getStringToSign(request), 'GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/?key=val ue with space');
    });

});
