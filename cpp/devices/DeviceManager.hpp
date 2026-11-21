
#pragma once

#include "DeviceDetector.hpp"
#include "../hardware/HardwareDetector.hpp"
#include "../hardware/HardwareProfileRegistry.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace swiftra1n::devices {

// ==========================================
// Unified USB Device
// ==========================================

struct UnifiedDevice {
    hardware::UsbDeviceInfo usb;
    hardware::HardwareClassification classification;
};

// ==========================================
// Device Manager Result
// ==========================================

struct DeviceManagerResult {
    // USB scanner status
    bool usbScanAvailable = false;
    std::string usbScanError;

    // USB statistics
    std::size_t usbDeviceCount = 0;
    std::size_t appleUsbDeviceCount = 0;
    std::size_t usbHubCount = 0;
    std::size_t knownBoardCount = 0;
    std::size_t unknownDeviceCount = 0;
    std::size_t descriptorErrors = 0;

    // Unified physical USB devices
    std::vector<UnifiedDevice> usbDevices;

    // Logical iOS devices
    std::vector<DeviceInfo> devices;
};

// ==========================================
// Device Manager
// ==========================================

class DeviceManager {
public:
    DeviceManagerResult scan() const;
};

}
