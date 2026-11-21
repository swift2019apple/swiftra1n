
#include "DeviceManager.hpp"

namespace swiftra1n::devices {

// ==========================================
// Unified Apple Device Scanner
// ==========================================

DeviceManagerResult DeviceManager::scan() const {
    DeviceManagerResult result;

    // Initialize hardware modules
    hardware::HardwareDetector usbDetector;
    hardware::HardwareProfileRegistry profileRegistry;

    // Scan physical USB hardware
    const auto usbScan = usbDetector.scan();

    result.usbScanAvailable = usbScan.success;
    result.usbScanError = usbScan.error;
    result.descriptorErrors = usbScan.descriptorErrors;

    // Keep only Apple USB devices
    if (usbScan.success) {
        for (const auto& usbDevice : usbScan.devices) {
            auto classification =
                profileRegistry.classify(usbDevice);

            if (classification.category !=
                hardware::HardwareCategory::appleDevice) {
                continue;
            }

            UnifiedDevice unified;
            unified.usb = usbDevice;
            unified.classification = classification;

            result.usbDevices.push_back(
                std::move(unified)
            );

            ++result.appleUsbDeviceCount;
        }
    }

    // Only Apple devices are included in this count
    result.usbDeviceCount = result.usbDevices.size();

    // Detect logical iOS devices
    DeviceDetector deviceDetector;
    result.devices = deviceDetector.detect();

    return result;
}

}
