
#include "../hardware/HardwareProfileRegistry.hpp"

#include <iostream>
#include <vector>

using namespace swiftra1n::hardware;

int main() {
    HardwareProfileRegistry registry;

    int passed = 0;
    int failed = 0;

    auto test = [&](const char* name,
                    const UsbDeviceInfo& device,
                    HardwareCategory expected,
                    bool expectedRecognized) {

        const auto result = registry.classify(device);

        const bool success =
            result.category == expected &&
            result.recognized == expectedRecognized;

        std::cout << (success ? "[PASS] " : "[FAIL] ")
                  << name << '\n';

        std::cout << "       Category: "
                  << hardwareCategoryName(result.category)
                  << '\n';

        std::cout << "       Match: "
                  << matchBasisName(result.matchBasis)
                  << "\n\n";

        success ? ++passed : ++failed;
    };

    UsbDeviceInfo iphone;
    iphone.vendorId = 0x05AC;
    iphone.productId = 0x12A8;
    iphone.manufacturer = "Apple Inc.";
    iphone.product = "iPhone";

    test(
        "Apple iPhone descriptor",
        iphone,
        HardwareCategory::appleDevice,
        true
    );

    UsbDeviceInfo appleUnknown;
    appleUnknown.vendorId = 0x05AC;

    test(
        "Apple device without product string",
        appleUnknown,
        HardwareCategory::appleDevice,
        true
    );

    UsbDeviceInfo hub;
    hub.vendorId = 0x1D6B;
    hub.productId = 0x0002;
    hub.deviceClass = 0x09;

    test(
        "USB hub",
        hub,
        HardwareCategory::usbHub,
        true
    );

    UsbDeviceInfo unknown;
    unknown.vendorId = 0xFFFF;
    unknown.productId = 0x0001;

    test(
        "Unregistered USB device",
        unknown,
        HardwareCategory::unknown,
        false
    );

    std::cout << "====================================\n";
    std::cout << "Hardware Profile Tests\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "====================================\n";

    return failed == 0 ? 0 : 1;
}
