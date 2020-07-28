<?php

namespace AlibabaCloud\Tea\RoaUtils\Tests;

use AlibabaCloud\Tea\Model;
use AlibabaCloud\Tea\Request;
use AlibabaCloud\Tea\RoaUtils\RoaUtils;
use PHPUnit\Framework\TestCase;

/**
 * @internal
 * @coversNothing
 */
class RoaUtilsTest extends TestCase
{
    /**
     * @var Request
     */
    private $request;

    public function setUp()
    {
        parent::setUp();
        $request                          = new Request();
        $request->pathname                = '/pathname';
        $request->headers['accept']       = 'application/json';
        $request->headers['content-type'] = 'application/text';
        $request->headers['date']         = '2020-02-24T02:23:06Z';

        $request->headers['x-acs-foo'] = 'bar';
        $request->headers['x-acs-bar'] = 'foo';

        $request->query['foo']    = 'bar';
        $request->query['number'] = 123;

        $this->request = $request;
    }

    public function testFilter()
    {
        $str = "a\t\n\r\fb";
        $res = RoaUtils::filter($str);
        $this->assertFalse(strpos($res, "\t"));
        $this->assertFalse(strpos($res, "\n"));
        $this->assertFalse(strpos($res, "\r"));
        $this->assertFalse(strpos($res, "\f"));
    }

    public function testGetCanonicalizedHeaders()
    {
        $headers = [
            'foo'       => 'bar',
            'x-acs-foo' => 'bar',
            'x-acs-bar' => 'foo',
        ];
        $this->assertEquals("x-acs-bar:foo\nx-acs-foo:bar\n", RoaUtils::getCanonicalizedHeaders($headers));
    }

    public function testGetCanonicalizedResource()
    {
        $pathname = '/pathname';
        $query    = [
            'foo'    => 'bar',
            'number' => 123,
            'empty'  => '',
        ];
        $this->assertEquals('/pathname?empty=&foo=bar&number=123', RoaUtils::getCanonicalizedResource($pathname, $query));
    }

    public function testGetStringToSign()
    {
        $request                    = new Request();
        $request->method            = 'GET';
        $request->pathname          = '/';
        $request->headers['accept'] = 'application/json';

        $this->assertEquals("GET\napplication/json\n\n\n\n/", RoaUtils::getStringToSign($request));

        $request->headers = [
            'accept'       => 'application/json',
            'content-md5'  => 'md5',
            'content-type' => 'application/json',
            'date'         => 'date',
        ];
        $this->assertEquals("GET\napplication/json\nmd5\napplication/json\ndate\n/", RoaUtils::getStringToSign($request));

        $request->headers = [
            'accept'           => 'application/json',
            'content-md5'      => 'md5',
            'content-type'     => 'application/json',
            'date'             => 'date',
            'x-acs-custom-key' => 'any value',
        ];
        $this->assertEquals("GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/", RoaUtils::getStringToSign($request));

        $request->query = [
            'key' => 'val ue with space',
        ];
        $this->assertEquals("GET\napplication/json\nmd5\napplication/json\ndate\nx-acs-custom-key:any value\n/?key=val ue with space", RoaUtils::getStringToSign($request));
    }

    public function testGetSignature()
    {
        $this->assertEquals('OmuTAr79tpI6CRoAdmzKRq5lHs0=', RoaUtils::getSignature('stringtosign', 'secret'));
    }

    public function testToForm()
    {
        $this->assertEquals('client=test&strs.1=str1&strs.2=str2&tag.key=value', RoaUtils::toForm([
            'client' => 'test',
            'tag'    => [
                'key' => 'value',
            ],
            'strs'   => ['str1', 'str2'],
        ]));
    }

    public function testConvert()
    {
        $model    = new MockModel();
        $model->a = 'foo';

        $output = new MockModel();
        RoaUtils::convert($model, $output);
        $this->assertEquals($model->a, $output->a);
    }
}

class MockModel extends Model
{
    public $a = 'A';

    public $b = '';

    public $c = '';

    public function __construct()
    {
        $this->_name['a']     = 'A';
        $this->_required['c'] = true;
    }
}
