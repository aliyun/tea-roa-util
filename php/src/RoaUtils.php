<?php

namespace AlibabaCloud\Tea\RoaUtils;

use Adbar\Dot;
use AlibabaCloud\Tea\Request;

class RoaUtils
{
    public static function filter($str)
    {
        return str_replace(["\t", "\n", "\r", "\f"], '', $str);
    }

    public static function getCanonicalizedHeaders($headers, $prefix = 'x-acs-')
    {
        ksort($headers);
        $str = '';
        foreach ($headers as $k => $v) {
            if (0 === strpos(strtolower($k), $prefix)) {
                $str .= $k . ':' . trim(self::filter($v)) . "\n";
            }
        }

        return $str;
    }

    public static function getCanonicalizedResource($pathname, $query)
    {
        if (0 === \count($query)) {
            return $pathname;
        }
        ksort($query);
        $tmp = [];
        foreach ($query as $k => $v) {
            array_push($tmp, $k . '=' . $v);
        }

        return $pathname . '?' . implode('&', $tmp);
    }

    public static function is4XXor5XX($code)
    {
        return $code >= 400 && $code < 600;
    }

    /**
     * @param Request $request
     *
     * @return string
     */
    public static function getStringToSign($request)
    {
        $pathname = $request->pathname ? $request->pathname : '';
        $headers  = new Dot($request->headers);
        $query    = $request->query ? $request->query : [];

        $result = $request->method . "\n" .
            $headers->get('accept', '') . "\n" .
            $headers->get('content-md5', '') . "\n" .
            $headers->get('content-type', '') . "\n" .
            $headers->get('date', '') . "\n";

        $canonicalizedHeaders  = self::getCanonicalizedHeaders($headers->get());
        $canonicalizedResource = self::getCanonicalizedResource($pathname, $query);

        return $result . $canonicalizedHeaders . $canonicalizedResource;
    }

    public static function getSignature($strToSign, $secret)
    {
        return base64_encode(hash_hmac('sha1', $strToSign, $secret, true));
    }

    public static function deleteSpecialKey($map, $key)
    {
        $res = [];
        foreach ($map as $k => $v) {
            if ($key !== $k) {
                $res[$k] = $v;
            }
        }

        return $res;
    }
}
