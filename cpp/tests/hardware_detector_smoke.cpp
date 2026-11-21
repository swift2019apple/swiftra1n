
#include "../hardware/HardwareDetector.hpp"
#include "../hardware/HardwareProfileRegistry.hpp"

#include <iostream>

using namespace swiftra1n::hardware;

int main() {
    HardwareDetector detector;
    HardwareProfileRegistry registry;

    const auto result = detector.scan();

    if (!result.success) {
        std::cerr << "[ERROR] " << result.error << '\n';
        return 1;
    }

    std::cout << "\n";
    std::cout << "================================================\n";
    std::cout << "          SwiftRa1n Hardware Scanner\n";
    std::cout << "================================================\n\n";

    std::cout << "USB devices detected: "
              << result.devices.size() << '\n';

    std::cout << "Descriptor errors: "
              << result.descriptorErrors << "\n\n";

    int number = 1;

    for (const auto& device : result.devices) {
        const auto classification = registry.classify(device);

        std::cout << "DEVICE #" << number++ << '\n';
        std::cout << "-----------------------------------------------\n";

        std::cout << "VID:PID: "
                  << formatUsbId(device.vendorId)
                  << ":"
                  << formatUsbId(device.productId)
                  << '\n';

        std::cout << "Bus: "
                  << static_cast<int>(device.busNumber) << '\n';

        std::cout << "Address: "
                  << static_cast<int>(device.address) << '\n';

        std::cout << "Manufacturer: "
                  << device.manufacturer << '\n';

        std::cout << "Product: "
                  << device.product << '\n';

        std::cout << "Category: "
                  << hardwareCategoryName(classification.category)
                  << '\n';

        std::cout << "Match basis: "
                  << matchBasisName(classification.matchBasis)
                  << '\n';

        std::cout << "Recognized: "
                  << (classification.recognized ? "YES" : "NO")
                  << '\n';

        std::cout << "USB speed: "
                  << usbSpeedName(device.speed)
                  << '\n';

        std::cout << "Strings accessible: "
                  << (device.stringsAccessible ? "YES" : "NO")
                  << '\n';

        if (!device.accessError.empty()) {
            std::cout << "Access note: "
                      << device.accessError << '\n';
        }

        std::cout << "Classification: "
                  << classification.reason << '\n';

        std::cout << "-----------------------------------------------\n\n";
    }

    std::cout << "USB scan completed.\n";
    std::cout << "No USB interfaces were claimed.\n";
    std::cout << "No device commands were sent.\n\n";

    return 0;
}
