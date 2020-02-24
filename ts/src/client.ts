// This file is auto-generated, don't edit it
import * as $tea from '@alicloud/tea-typescript';
import kitx from 'kitx';

function filter(value: string): string {
  return value.replace(/[\t\n\r\f]/g, ' ');
}

function getCanonicalizedHeaders(headers: {[key: string]: string}): string {
  const prefix = 'x-acs-';
  const keys = Object.keys(headers);

  const canonicalizedKeys = [];
  for (let i = 0; i < keys.length; i++) {
    const key = keys[i];
    if (key.startsWith(prefix)) {
      canonicalizedKeys.push(key);
    }
  }

  canonicalizedKeys.sort();

  var result = '';
  for (let i = 0; i < canonicalizedKeys.length; i++) {
    const key = canonicalizedKeys[i];
    result += `${key}:${filter(headers[key]).trim()}\n`;
  }

  return result;
}

function getCanonicalizedResource(uriPattern: string, query: {[key: string]: string}): string {
  const keys = Object.keys(query).sort();

  if (keys.length === 0) {
    return uriPattern;
  }

  var result = [];
  for (var i = 0; i < keys.length; i++) {
    const key = keys[i];
    result.push(`${key}=${query[key]}`);
  }

  return `${uriPattern}?${result.join('&')}`;
}

export default class Client {

  static is4XXor5XX(code: number): boolean {
    return code >= 400 && code < 600;
  }

  static getStringToSign(request: $tea.Request): string {
    const method = request.method;
    const accept = request.headers['accept'];
    const contentMD5 = request.headers['content-md5'] || '';
    const contentType = request.headers['content-type'] || '';
    const date = request.headers['date'] || '';
    const header = `${method}\n${accept}\n${contentMD5}\n${contentType}\n${date}\n`;
    const canonicalizedHeaders = getCanonicalizedHeaders(request.headers);
    const canonicalizedResource = getCanonicalizedResource(request.pathname, request.query);
  
    return `${header}${canonicalizedHeaders}${canonicalizedResource}`;
  }

  static getSignature(stringToSign: string, secret: string): string {
      const utf8Buff = Buffer.from(stringToSign, 'utf8');
      return kitx.sha1(utf8Buff, secret, 'base64') as string;
  }

}
