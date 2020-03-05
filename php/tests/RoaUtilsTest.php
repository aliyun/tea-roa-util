<?php

namespace AlibabaCloud\Tea\Tests;

use AlibabaCloud\Tea\Request;
use AlibabaCloud\Tea\RoaUtils;
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
        $this->assertFalse(strpos("\t", $res));
        $this->assertFalse(strpos("\n", $res));
        $this->assertFalse(strpos("\r", $res));
        $this->assertFalse(strpos("\f", $res));
    }

    public function testGetCanonicalizedHeaders()
    {
        $headers = [
            'foo'       => 'bar',
            'x-acs-foo' => 'bar',
            'x-acs-bar' => 'foo',
        ];
        $this->assertEquals("x-acs-bar:foo\nx-acs-foo:bar", RoaUtils::getCanonicalizedHeaders($headers));
    }

    public function testGetCanonicalizedResource()
    {
        $pathname = '/pathname';
        $query    = [
            'foo'    => 'bar',
            'number' => 123,
            'empty'  => '',
        ];
        $this->assertEquals('/pathname?empty=foo=barnumber=123', RoaUtils::getCanonicalizedResource($pathname, $query));
    }

    public function testIs4XXor5XX()
    {
        $this->assertTrue(RoaUtils::is4XXor5XX(404));
        $this->assertFalse(RoaUtils::is4XXor5XX(200));
    }

    public function testGetStringToSign()
    {
        $this->assertEquals(
            "GET\napplication/json\n\napplication/text\n2020-02-24T02:23:06Z\nx-acs-bar:foo\nx-acs-foo:bar\n/pathname?foo=barnumber=123",
            RoaUtils:: getStringToSign($this->request)
        );
    }

    public function testGetSignature()
    {
        $strToSign = RoaUtils::getStringToSign($this->request);
        $this->assertEquals('yRemXCsUeqd7gB+7UXZATv7pugQ=', RoaUtils::getSignature($strToSign, 'secret'));
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
