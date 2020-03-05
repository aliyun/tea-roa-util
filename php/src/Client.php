<?php

namespace AlibabaCloud\Tea\RoaUtils;

use AlibabaCloud\Tea\Request;

class Client
{
    public static function filter($str)
    {
        return str_replace(["\t", "\n", "\r", "\f"], "", $str);
    }

    public static function getCanonicalizedHeaders($headers, $prefix = "x-acs-")
    {
        ksort($headers);
        $tmp = [];
        foreach ($headers as $k => $v) {
            if (false !== strpos($k, $prefix)) {
                array_push($tmp, $k . ":" . trim($v));
            }
        }
        return implode("\n", $tmp);
    }

    public static function getCanonicalizedResource($pathname, $query)
    {
        if (count($query) === 0) {
            return $pathname;
        }
        $result = $pathname . "?";
        ksort($query);
        foreach ($query as $k => $v) {
            $result = $result . $k . "=" . $v;
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
        $pathname = $request->pathname ? $request->pathname : "";
        $headers  = $request->headers ? $request->headers : [];
        $query    = $request->query ? $request->query : [];

        $headerValue = [
            $request->method,
            self::readArray($headers, "accept"),
            self::readArray($headers, "content-md5"),
            self::readArray($headers, "content-type"),
            self::readArray($headers, "date")
        ];
        return implode("\n", [
            implode("\n", $headerValue),
            self::getCanonicalizedHeaders($headers),
            self::getCanonicalizedResource($pathname, $query)
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

    private static function readArray($array, $key, $default = "")
    {
        return null !== $array && !empty($array) && isset($array[$key]) ? $array[$key] : $default;
    }
}