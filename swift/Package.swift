// swift-tools-version:5.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "AlibabaCloudRoaUtils",
    products: [
        .library(
            name: "AlibabaCloudRoaUtils",
            targets: ["AlibabaCloudRoaUtils"])
    ],
    dependencies: [
        .package(url: "https://github.com/aliyun/tea-swift.git", from: "0.3.0")
    ],
    targets: [
        .target(
            name: "AlibabaCloudRoaUtils",
            dependencies: ["Tea"]),
        .testTarget(
            name: "AlibabaCloudRoaUtilsTests",
            dependencies: ["AlibabaCloudRoaUtils", "Tea"])
    ]
)
