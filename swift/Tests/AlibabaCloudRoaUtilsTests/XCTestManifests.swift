import XCTest

#if !canImport(ObjectiveC)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(AlibabaCloudRoaUtilsTests.allTests),
    ]
}
#endif
