import Foundation
import XCTest
import AlamofirePromiseKit
import Alamofire
import AwaitKit
import Tea
@testable import AlibabaCloudRoaUtils

final class AlibabaCloudRoaUtilsTests: XCTestCase {
    var request: TeaRequest = TeaRequest()

    override func setUp() {
        super.setUp()
        request.pathname = "/pathname"
        request.headers["accept"] = "application/json"
        request.headers["content-type"] = "application/text"
        request.headers["date"] = "2020-02-24T02:23:06Z"

        request.headers["x-acs-foo"] = "bar"
        request.headers["x-acs-bar"] = "foo"

        request.query["foo"] = "bar"
        request.query["number"] = 123
    }

    func testFilter() {
        let str: String = "a\t\n\rb"
        let result = AlibabaCloudRoaUtils.filter(str)
        XCTAssertEqual("a   b", result)
        XCTAssertFalse(result.contains("\t"))
        XCTAssertFalse(result.contains("\n"))
        XCTAssertFalse(result.contains("\r"))
    }

    func testGetCanonicalizedHeaders() {
        let headers: [String: String] = [
            "foo": "bar",
            "x-acs-foo": "bar",
            "x-acs-bar": "foo"
        ]
        XCTAssertEqual("x-acs-bar:foo\nx-acs-foo:bar", AlibabaCloudRoaUtils.getCanonicalizedHeaders(headers))
    }

    func testGetCanonicalizedResource() {
        let pathname: String = "/pathname"
        let query: [String: Any] = [
            "foo": "bar",
            "number": 123,
            "empty": "",
        ]
        XCTAssertEqual("/pathname?empty=foo=barnumber=123", AlibabaCloudRoaUtils.getCanonicalizedResource(pathname, query))
    }

    func testIs4XXor5XX() {
        XCTAssertTrue(AlibabaCloudRoaUtils.is4XXor5XX(404))
        XCTAssertFalse(AlibabaCloudRoaUtils.is4XXor5XX(200))
    }

    func testGetStringToSign() {
        XCTAssertEqual("GET\napplication/json\n\napplication/text\n2020-02-24T02:23:06Z\nx-acs-bar:foo\nx-acs-foo:bar\n/pathname?foo=barnumber=123", AlibabaCloudRoaUtils.getStringToSign(request))
    }

    func testGetSignature() {
        let strToSign = AlibabaCloudRoaUtils.getStringToSign(request)
        XCTAssertEqual("yRemXCsUeqd7gB+7UXZATv7pugQ=", AlibabaCloudRoaUtils.getSignature(strToSign, "secret"))
    }

    func testDeleteSpecialKey() {
        let source: [String: String] = [
            "foo": "bar",
            "delete": "for delete"
        ]
        let target: [String: String] = [
            "foo": "bar"
        ]

        let res: [String: String] = AlibabaCloudRoaUtils.deleteSpecialKey(source, "delete") as! [String: String]

        XCTAssertEqual(target, res)
        XCTAssertEqual([
            "foo": "bar",
            "delete": "for delete"
        ], source)
    }

    static var allTests = [
        ("testFilter", testFilter),
        ("testGetCanonicalizedHeaders", testGetCanonicalizedHeaders),
        ("testGetCanonicalizedResource", testGetCanonicalizedResource),
        ("testIs4XXor5XX", testIs4XXor5XX),
        ("testGetStringToSign", testGetStringToSign),
        ("testGetSignature", testGetSignature),
    ]
}