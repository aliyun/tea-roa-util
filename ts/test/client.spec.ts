import Client from '../src/client';

import { Readable } from 'stream';
import * as $tea from '@alicloud/tea-typescript';
import assert from 'assert';
import 'mocha';

describe('Tea Util', function () {
  it('Module should ok', function () {
    assert.ok(Client);
  });

  it('covert should ok', function () {
    class SubGrant extends $tea.Model {
      grant: string;
      other: string;
      static names(): { [key: string]: string } {
        return {
          grant: 'Grant',
          other: 'Other',
        };
      }

      static types(): { [key: string]: any } {
        return {
          grant: 'string',
          other: 'string',
        };
      }

      constructor(map: { [key: string]: any }) {
        super(map);
      }
    }

    class SubGrantBak extends $tea.Model {
      grant: string;
      diff: string;
      static names(): { [key: string]: string } {
        return {
          grant: 'Grant',
          diff: 'Diff'
        };
      }

      static types(): { [key: string]: any } {
        return {
          grant: 'string',
          diff: 'string',
        };
      }

      constructor(map: { [key: string]: any }) {
        super(map);
      }
    }

    class Grant extends $tea.Model {
        subGrant: SubGrant;
        static names(): { [key: string]: string } {
          return {
            subGrant: 'SubGrant',
          };
        }

        static types(): { [key: string]: any } {
          return {
            subGrant: SubGrant,
          };
        }

        constructor(map: { [key: string]: any }) {
          super(map);
        }
    }

    class GrantBak extends $tea.Model {
      subGrant: SubGrantBak;
      static names(): { [key: string]: string } {
        return {
          subGrant: 'SubGrant',
        };
      }

      static types(): { [key: string]: any } {
        return {
          subGrant: SubGrantBak,
        };
      }

      constructor(map: { [key: string]: any }) {
        super(map);
      }
    }
    let inputModel: $tea.Model = new Grant({
      subGrant: new SubGrant({ grant: 'test', other: 'other'}),
    });
    let outputModel: $tea.Model = new GrantBak({
      subGrant: new SubGrantBak({ grant: 'test', diff: 'diff' }),
    });
    Client.convert(inputModel, outputModel);
    assert.strictEqual(outputModel.subGrant.grant, 'test');
    assert.strictEqual(outputModel.subGrant.other, undefined);
    assert.strictEqual(outputModel.subGrant.diff, 'diff');
    outputModel = new GrantBak({});
    Client.convert(inputModel, outputModel);
    assert.strictEqual(outputModel.subGrant.grant, 'test');
    assert.strictEqual(outputModel.subGrant.other, undefined);
    assert.strictEqual(outputModel.subGrant.diff, undefined);
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

  it('toForm', function (){
    const data: { [key: string]: any } = {
      val1: 'string',
      val2: undefined,
      val3: null,
      val4: 1,
      val5: true,
      val6: {
        subval1: 'string',
        subval2: 1,
        subval3: true,
        subval4: null,
        subval5: [
          '1',
          2,
          true,
          {
            val1: 'string'
          }
        ],
      },
      val7: [
        '1',
        2,
        undefined,
        null,
        true,
        {
          val1: 'string'
        },
        [
          'substring'
        ]
      ]
    };
    assert.deepStrictEqual(Client.toForm(data), 'val1=string&val2=&val3=&val4=1&val5=true&val6.subval1=string&val6.subval2=1&val6.subval3=true&val6.subval4=&val6.subval5.1=1&val6.subval5.2=2&val6.subval5.3=true&val6.subval5.4.val1=string&val7.1=1&val7.2=2&val7.3=&val7.4=&val7.5=true&val7.6.val1=string&val7.7.1=substring');
    assert.deepStrictEqual(Client.toForm(undefined), '');
  })

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
