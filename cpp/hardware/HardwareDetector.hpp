
#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace swiftra1n::hardware {

struct UsbDeviceInfo {
    std::uint8_t busNumber = 0;
    std::uint8_t address = 0;

    std::vector<std::uint8_t> portPath;

    std::uint16_t vendorId = 0;
    std::uint16_t productId = 0;

    std::uint8_t deviceClass = 0;
    std::uint8_t deviceSubClass = 0;
    std::uint8_t deviceProtocol = 0;

    std::uint8_t configurations = 0;
    int speed = 0;

    std::string manufacturer = "Unavailable";
    std::string product = "Unavailable";

    // USB serial numbers are intentionally not read.
    // They are not needed for generic hardware identification.
    bool stringsAccessible = false;
    std::string accessError;
};

struct UsbScanResult {
    bool success = false;
    std::string error;

    std::size_t descriptorErrors = 0;
    std::vector<UsbDeviceInfo> devices;
};

class HardwareDetector {
public:
    UsbScanResult scan() const;
};

std::string formatUsbId(std::uint16_t value);
std::string usbSpeedName(int speed);

}
