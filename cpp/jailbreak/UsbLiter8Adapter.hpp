#pragma once

#include <string>
#include <vector>

namespace swiftra1n::jailbreak {

enum class UsbLiter8Status {
    boardNotFound,
    serialCandidateFound
};

struct UsbSerialPortInfo {
    std::string devicePath;
    std::string vendorId = "Unknown";
    std::string productId = "Unknown";
    std::string manufacturer = "Unknown";
    std::string product = "Unknown";
    std::string serialNumber = "Unknown";
};

struct UsbLiter8Assessment {
    UsbLiter8Status status;
    bool boardIdentityVerified = false;
    bool canLaunch = false;

    std::vector<UsbSerialPortInfo> ports;

    std::string reason;
};

class UsbLiter8Adapter {
public:
    std::vector<UsbSerialPortInfo> enumerateSerialPorts() const;

    UsbLiter8Assessment assess(
        const std::vector<UsbSerialPortInfo>& ports
    ) const;

    UsbLiter8Assessment inspect() const;
};

const char* usbLiter8StatusName(UsbLiter8Status status);

}
