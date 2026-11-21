#include "UsbLiter8Adapter.hpp"

#include <filesystem>
#include <fstream>
#include <set>
#include <system_error>

namespace swiftra1n::jailbreak {

namespace fs = std::filesystem;

namespace {

// ==========================================
// Read sysfs metadata
// ==========================================

std::string readTextFile(const fs::path& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return "Unknown";
    }

    std::string value;
    std::getline(file, value);

    if (value.empty()) {
        return "Unknown";
    }

    return value;
}

void readUsbMetadata(
    const fs::path& devicePath,
    UsbSerialPortInfo& info
) {
    const std::string ttyName = devicePath.filename().string();

    fs::path sysfsPath =
        fs::path("/sys/class/tty") / ttyName / "device";

    std::error_code error;
    sysfsPath = fs::canonical(sysfsPath, error);

    if (error) {
        return;
    }

    // Walk up sysfs until USB device descriptors are found.
    for (fs::path current = sysfsPath;
         !current.empty() && current != current.root_path();
         current = current.parent_path()) {

        const fs::path vendorFile = current / "idVendor";
        const fs::path productFile = current / "idProduct";

        if (fs::exists(vendorFile, error) &&
            fs::exists(productFile, error)) {

            info.vendorId = readTextFile(vendorFile);
            info.productId = readTextFile(productFile);
            info.manufacturer =
                readTextFile(current / "manufacturer");
            info.product =
                readTextFile(current / "product");
            info.serialNumber =
                readTextFile(current / "serial");

            return;
        }

        error.clear();
    }
}

// ==========================================
// Add serial port without duplicates
// ==========================================

void addPort(
    const fs::path& path,
    std::set<std::string>& knownDevices,
    std::vector<UsbSerialPortInfo>& result
) {
    std::error_code error;

    const fs::path canonicalPath =
        fs::canonical(path, error);

    if (error) {
        return;
    }

    const std::string resolvedPath = canonicalPath.string();

    if (!knownDevices.insert(resolvedPath).second) {
        return;
    }

    UsbSerialPortInfo info;

    // Keep the stable by-id path when available.
    info.devicePath = path.string();

    readUsbMetadata(canonicalPath, info);

    result.push_back(info);
}

bool startsWith(
    const std::string& value,
    const std::string& prefix
) {
    return value.rfind(prefix, 0) == 0;
}

} // namespace

// ==========================================
// Enumerate serial interfaces
// ==========================================

std::vector<UsbSerialPortInfo>
UsbLiter8Adapter::enumerateSerialPorts() const {

    std::vector<UsbSerialPortInfo> result;
    std::set<std::string> knownDevices;

    // Stable USB serial names.
    const fs::path byIdDirectory("/dev/serial/by-id");

    std::error_code error;

    if (fs::is_directory(byIdDirectory, error)) {
        for (const auto& entry :
             fs::directory_iterator(byIdDirectory, error)) {

            if (error) {
                break;
            }

            addPort(entry.path(), knownDevices, result);
        }
    }

    error.clear();

    // Common Linux USB serial interfaces.
    const fs::path devDirectory("/dev");

    if (fs::is_directory(devDirectory, error)) {
        for (const auto& entry :
             fs::directory_iterator(devDirectory, error)) {

            if (error) {
                break;
            }

            const std::string name =
                entry.path().filename().string();

            if (startsWith(name, "ttyACM") ||
                startsWith(name, "ttyUSB")) {

                addPort(entry.path(), knownDevices, result);
            }
        }
    }

    return result;
}

// ==========================================
// Assess detected interfaces
// ==========================================

UsbLiter8Assessment UsbLiter8Adapter::assess(
    const std::vector<UsbSerialPortInfo>& ports
) const {

    if (ports.empty()) {
        return {
            UsbLiter8Status::boardNotFound,
            false,
            false,
            {},
            "No USB serial interfaces found. "
            "The RP2350 board may be disconnected, "
            "or its console interface may not be active."
        };
    }

    return {
        UsbLiter8Status::serialCandidateFound,
        false,
        false,
        ports,
        "USB serial interface(s) found. This is only a candidate: "
        "the interface has not been authenticated as an RP2350 "
        "running usbliter8 firmware."
    };
}

// ==========================================
// Full inspection
// ==========================================

UsbLiter8Assessment UsbLiter8Adapter::inspect() const {
    return assess(enumerateSerialPorts());
}

// ==========================================
// Status names
// ==========================================

const char* usbLiter8StatusName(UsbLiter8Status status) {
    switch (status) {
        case UsbLiter8Status::boardNotFound:
            return "BOARD_NOT_FOUND";

        case UsbLiter8Status::serialCandidateFound:
            return "SERIAL_CANDIDATE_FOUND";
    }

    return "UNKNOWN";
}

} // namespace swiftra1n::jailbreak
