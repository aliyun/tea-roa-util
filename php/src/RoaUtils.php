<?php

namespace AlibabaCloud\Tea\RoaUtils;

use AlibabaCloud\Tea\Model;
use AlibabaCloud\Tea\Request;
use Psr\Http\Message\StreamInterface;

/**
 * This is for ROA SDK.
 */
class RoaUtils
{
    /**
     * Get the string to be signed according to request.
     *
     * @param Request $request which contains signed messages
     *
     * @return string the signed string
     */
    public static function getStringToSign($request)
    {
        $pathname = $request->pathname ? $request->pathname : '';
        $query    = $request->query ? $request->query : [];

        $accept      = isset($request->headers['accept']) ? $request->headers['accept'] : '';
        $contentMD5  = isset($request->headers['content-md5']) ? $request->headers['content-md5'] : '';
        $contentType = isset($request->headers['content-type']) ? $request->headers['content-type'] : '';
        $date        = isset($request->headers['date']) ? $request->headers['date'] : '';

        $result = $request->method . "\n" .
            $accept . "\n" .
            $contentMD5 . "\n" .
            $contentType . "\n" .
            $date . "\n";

        $canonicalizedHeaders  = self::getCanonicalizedHeaders($request->headers);
        $canonicalizedResource = self::getCanonicalizedResource($pathname, $query);

        return $result . $canonicalizedHeaders . $canonicalizedResource;
    }

    /**
     * Get signature according to stringToSign, secret.
     *
     * @param string $stringToSign the signed string
     * @param string $secret       accesskey secret
     *
     * @return string the signature
     */
    public static function getSignature($stringToSign, $secret)
    {
        return base64_encode(hash_hmac('sha1', $stringToSign, $secret, true));
    }

    /**
     * Parse filter into a form string.
     *
     * @param array $query object
     *
     * @return string the string
     */
    public static function toForm($query)
    {
        if (null === $query) {
            return '';
        }
        if ($query instanceof Model) {
            $query = $query->toMap();
        }
        $tmp = [];
        foreach ($query as $k => $v) {
            if (0 !== strpos($k, '_')) {
                $tmp[$k] = $v;
            }
        }
        $res = self::flatten($tmp);
        ksort($res);

        return http_build_query($res);
    }

    /**
     * Convert all params of body other than type of readable into content.
     *
     * @param Model $body    source Model
     * @param Model $content target Model
     *
     * @throws \ReflectionException
     */
    public static function convert($body, &$content)
    {
        $class = new \ReflectionClass($body);
        foreach ($class->getProperties(\ReflectionProperty::IS_PUBLIC) as $property) {
            $name = $property->getName();
            if (!$property->isStatic()) {
                $value = $property->getValue($body);
                if ($value instanceof StreamInterface) {
                    continue;
                }
                $content->{$name} = $value;
            }
        }
    }

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

    private static function flatten($items = [], $delimiter = '.', $prepend = '')
    {
        $flatten = [];

        foreach ($items as $key => $value) {
            $pos = \is_int($key) ? $key + 1 : $key;
            if (\is_array($value) && !empty($value)) {
                $flatten = array_merge(
                    $flatten,
                    self::flatten($value, $delimiter, $prepend . $pos . $delimiter)
                );
            } else {
                $flatten[$prepend . $pos] = $value;
            }
        }

        return $flatten;
    }
}
