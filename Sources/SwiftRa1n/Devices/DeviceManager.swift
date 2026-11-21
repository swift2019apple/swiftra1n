import Foundation

final class DeviceManager {

    private(set) var devices: [AppleDevice] = []

    // Добавление обнаруженного устройства
    func addDevice(_ device: AppleDevice) {
        guard !devices.contains(where: {
            $0.identifier == device.identifier
        }) else {
            return
        }

        devices.append(device)
        print("[+] Device added: \(device.displayName)")
    }

    // Удаление устройства
    func removeDevice(identifier: String) {
        devices.removeAll {
            $0.identifier == identifier
        }

        print("[-] Device removed: \(identifier)")
    }

    // Очистка списка
    func clearDevices() {
        devices.removeAll()
    }

    // Получение списка устройств
    func getDevices() -> [AppleDevice] {
        devices
    }

    // Проверка возможности jailbreak
    func canJailbreak(_ device: AppleDevice) -> Bool {
        device.jailbreakStatus == .confirmed
    }

    // Тестовые устройства
    func loadMockDevices() {

        devices.removeAll()

        let iphone11 = AppleDevice(
            identifier: "MOCK-IPHONE-11",
            productType: "iPhone12,1",
            displayName: "iPhone 11",
            family: .iPhone,
            chip: .A13,
            osVersion: "Unknown",
            mode: .normal,
            jailbreakStatus: .untested
        )

        let ipad = AppleDevice(
            identifier: "MOCK-IPAD",
            productType: "iPad",
            displayName: "Test iPad",
            family: .iPad,
            chip: .unknown,
            osVersion: "Unknown",
            mode: .normal,
            jailbreakStatus: .untested
        )

        addDevice(iphone11)
        addDevice(ipad)
    }
}
