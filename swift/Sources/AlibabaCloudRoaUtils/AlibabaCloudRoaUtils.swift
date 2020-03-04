import Foundation
import Tea
import CryptoSwift

public class AlibabaCloudRoaUtils {
    static func filter(_ str: String) -> String {
        str.replacingOccurrences(of: "\t", with: " ")
                .replacingOccurrences(of: "\n", with: " ")
                .replacingOccurrences(of: "\r", with: " ")
                .replacingOccurrences(of: "\\f", with: " ")
    }

    static func getCanonicalizedHeaders(_ headers: [String: String]) -> String {
        let prefix: String = "x-acs-";
        var canonicalizedKeys: [String] = [String]();
        for (key, _) in headers {
            if (key.hasPrefix(prefix)) {
                canonicalizedKeys.append(key);
            }
        }
        canonicalizedKeys.sort();
        var result: String = "";
        var n = 0;
        for key in canonicalizedKeys {
            if n != 0 {
                result.append(contentsOf: "\n");
            }
            result.append(contentsOf: key);
            result.append(contentsOf: ":");
            result.append(contentsOf: headers[key]?.trimmingCharacters(in: .whitespacesAndNewlines) ?? "");
            n += 1;
        }
        return result;
    }

    static func getCanonicalizedResource(_ pathname: String, _ query: [String: Any]) -> String {
        if (query.count <= 0) {
            return pathname;
        }
        var keys: [String] = [String]();
        for (key, _) in query {
            keys.append(key);
        }
        keys.sort();
        var result: String = pathname + "?";
        for key in keys {
            result.append(key);
            result.append("=");
            result.append("\(query[key] ?? "")");
        }
        return result;
    }

    static func is4XXor5XX(_ code: Int) -> Bool {
        code >= 400 && code < 600;
    }

    static func getStringToSign(_ request: TeaRequest) -> String {
        let method: String = request.method;
        let pathname: String = request.pathname;
        let headers: [String: String] = request.headers;
        let query: [String: Any] = request.query;

        let accept = headers["accept"] ?? "";
        let contentMD5 = headers["content-md5"] ?? "";
        let contentType = headers["content-type"] ?? "";
        let date = headers["date"] ?? "";

        let headerStr = String(method) + "\n" + String(accept) + "\n" + String(contentMD5) + "\n" + String(contentType) + "\n" + String(date);
        let canonicalizedHeaders: String = getCanonicalizedHeaders(headers);
        let canonicalizedResource: String = getCanonicalizedResource(pathname, query);
        return headerStr + "\n" + canonicalizedHeaders + "\n" + canonicalizedResource;
    }

    static func getSignature(_ strToSign: String, _ secret: String) -> String {
        let r: [UInt8] = try! HMAC(key: secret, variant: .sha1).authenticate(strToSign.bytes)
        return r.toBase64() ?? "";
    }

    static func deleteSpecialKey(_ dict: [String: Any], _ key: String) -> [String: Any] {
        var res: [String: Any] = [String: Any]()
        for (k, v) in dict {
            if key != k {
                res[k] = v
            }
        }
        return res
    }
}
