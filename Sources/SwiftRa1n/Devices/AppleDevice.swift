import Foundation

// MARK: - Device Family

enum DeviceFamily: String, Codable, Sendable {
    case iPhone
    case iPad
    case unknown
}

// MARK: - Apple Chip

enum AppleChip: String, Codable, Sendable {
    case A8
    case A9
    case A10
    case A11
    case A12
    case A12X
    case A12Z
    case A13
    case A14
    case A15
    case A16
    case A17
    case A18
    case A19
    case unknown
}

// MARK: - Connection Mode

enum ConnectionMode: String, Codable, Sendable {
    case normal
    case recovery
    case dfu
    case disconnected
    case unknown
}

// MARK: - Jailbreak Status

enum JailbreakStatus: String, Codable, Sendable {
    case confirmed
    case unsupported
    case untested
    case unavailable
}

// MARK: - Apple Device

struct AppleDevice: Identifiable, Codable, Sendable {

    let identifier: String
    let productType: String
    let displayName: String

    let family: DeviceFamily
    let chip: AppleChip

    let osVersion: String
    let mode: ConnectionMode

    let jailbreakStatus: JailbreakStatus

    var id: String {
        identifier
    }
}
