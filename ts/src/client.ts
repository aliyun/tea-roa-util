// This file is auto-generated, don't edit it
import * as $tea from '@alicloud/tea-typescript';
import Util from '@alicloud/tea-util';
import kitx from 'kitx';

function replaceRepeatList(target: { [key: string]: string }, repeat: any[], prefix: string) {
  if (prefix) {
    prefix = prefix + '.';
  }
  for (var i = 0; i < repeat.length; i++) {
    var item = repeat[i];
    let key = prefix + (i + 1);
    if (typeof item === 'undefined' || item == null) {
      target[key] = '';
      continue;
    }
    if (Array.isArray(item)) {
      replaceRepeatList(target, item, key);
    } else if (item instanceof Object) {
      flatMap(target, item, key);
    } else {
      target[key] = item.toString();
    }
  }
}

function flatMap(target: { [key: string]: any }, params: { [key: string]: any }, prefix: string = '') {
  if (prefix) {
    prefix = prefix + '.';
  }
  let keys = Object.keys(params);
  for (let i = 0; i < keys.length; i++) {
    let key = keys[i];
    let value = params[key];
    key = prefix + key;
    if (typeof value === 'undefined' || value == null) {
      target[key] = '';
      continue;
    }

    if (Array.isArray(value)) {
      replaceRepeatList(target, value, key);
    } else if (value instanceof Object) {
      flatMap(target, value, key);
    } else {
      target[key] = value.toString();
    }
  }
  return target;
}

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

  static toForm(filter: { [key: string]: any }): string {
    let target = {};
    flatMap(target, filter);
    return Util.toFormString(target);
  }
}
