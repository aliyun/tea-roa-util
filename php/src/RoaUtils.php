<?php

namespace AlibabaCloud\Tea;

use Adbar\Dot;

class RoaUtils
{
    public static function filter($str)
    {
        return str_replace(["\t", "\n", "\r", "\f"], '', $str);
    }

    public static function getCanonicalizedHeaders($headers, $prefix = 'x-acs-')
    {
        ksort($headers);
        $tmp = [];
        foreach ($headers as $k => $v) {
            if (false !== strpos($k, $prefix)) {
                array_push($tmp, $k . ':' . trim($v));
            }
        }

        return implode("\n", $tmp);
    }

    public static function getCanonicalizedResource($pathname, $query)
    {
        if (0 === \count($query)) {
            return $pathname;
        }
        $result = $pathname . '?';
        ksort($query);
        foreach ($query as $k => $v) {
            $result = $result . $k . '=' . $v;
        }

        return $result;
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

        $headerValue = [
            $request->method,
            $headers->get('accept', ''),
            $headers->get('content-md5', ''),
            $headers->get('content-type', ''),
            $headers->get('date', ''),
        ];

        return implode("\n", [
            implode("\n", $headerValue),
            self::getCanonicalizedHeaders($headers->get()),
            self::getCanonicalizedResource($pathname, $query),
        ]);
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
