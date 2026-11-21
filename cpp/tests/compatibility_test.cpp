#include "../jailbreak/Compatibility.hpp"

#include <iostream>
#include <string>

using namespace swiftra1n;

devices::DeviceInfo makeMockDevice(
    const std::string& model,
    const std::string& chip,
    const std::string& ios,
    const std::string& build = "Unknown"
) {
    devices::DeviceInfo device;

    device.identifier = "MOCK-" + model;
    device.productType = model;
    device.family = model.starts_with("iPad") ? "iPad" : "iPhone";
    device.chip = chip;
    device.osVersion = ios;
    device.buildVersion = build;
    device.mode = "MOCK";

    return device;
}

bool runTest(
    const std::string& name,
    const devices::DeviceInfo& device,
    jailbreak::CompatibilityStatus expected
) {
    jailbreak::CompatibilityChecker checker;
    const auto result = checker.check(device);

    const bool passed = result.status == expected;

    std::cout << (passed ? "[PASS] " : "[FAIL] ")
              << name << '\n';

    std::cout << "       Expected: "
              << jailbreak::statusName(expected) << '\n';

    std::cout << "       Actual:   "
              << jailbreak::statusName(result.status) << '\n';

    std::cout << "       Reason:   "
              << result.reason << "\n\n";

    return passed;
}

int main() {

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "  SwiftRa1n Compatibility Tests\n";
    std::cout << "====================================\n\n";

    int passed = 0;
    int failed = 0;

    auto test = [&](const std::string& name,
                    const devices::DeviceInfo& device,
                    jailbreak::CompatibilityStatus expected) {

        if (runTest(name, device, expected)) {
            ++passed;
        } else {
            ++failed;
        }
    };

    test(
        "iPhone 11 / A13 / missing build",
        makeMockDevice("iPhone12,1", "A13", "17.0.0"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "iPhone XS / A12 / iOS 16",
        makeMockDevice("iPhone11,2", "A12", "16.0.0"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "iPhone XS / A12 / iOS 12.0",
        makeMockDevice("iPhone11,2", "A12", "12.0", "UNKNOWN_BUILD"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "iPhone XR / OS before first release",
        makeMockDevice("iPhone11,8", "A12", "12.0", "UNKNOWN_BUILD"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "iPhone 11 / exact research profile",
        makeMockDevice("iPhone12,1", "A13", "27.2", "24B119"),
        jailbreak::CompatibilityStatus::researchOnly
    );

    test(
        "iPhone 11 / unknown build",
        makeMockDevice("iPhone12,1", "A13", "27.2", "UNKNOWN_BUILD"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "Device outside target / A15",
        makeMockDevice("iPhone14,2", "A15", "17.0.0"),
        jailbreak::CompatibilityStatus::unsupported
    );

    test(
        "Unknown device model",
        makeMockDevice("Unknown", "A13", "17.0.0"),
        jailbreak::CompatibilityStatus::untested
    );

    test(
        "Invalid iOS version",
        makeMockDevice("iPhone12,1", "A13", "Unknown"),
        jailbreak::CompatibilityStatus::untested
    );

    std::cout << "====================================\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "====================================\n";

    return failed == 0 ? 0 : 1;
}
