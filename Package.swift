// swift-tools-version: 6.0

import PackageDescription

let package = Package(
    name: "SwiftRa1n",
    products: [
        .executable(
            name: "SwiftRa1n",
            targets: ["SwiftRa1n"]
        )
    ],
    targets: [
        .executableTarget(
            name: "SwiftRa1n"
        ),
        .testTarget(
            name: "SwiftRa1nTests",
            dependencies: ["SwiftRa1n"]
        )
    ]
)
