<?php

namespace AlibabaCloud\Tea\RoaUtils\Tests;

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

    public function testIs4XXor5XX()
    {
        $this->assertTrue(RoaUtils::is4XXor5XX(404));
        $this->assertFalse(RoaUtils::is4XXor5XX(200));
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

    public function testDeleteSpecialKey()
    {
        $source = [
            'foo'    => 'bar',
            'delete' => 'for delete',
        ];
        $target = [
            'foo' => 'bar',
        ];
        $res    = RoaUtils::deleteSpecialKey($source, 'delete');
        $this->assertEquals($target, $res);
        $this->assertEquals([
            'foo'    => 'bar',
            'delete' => 'for delete',
        ], $source);
    }
}
