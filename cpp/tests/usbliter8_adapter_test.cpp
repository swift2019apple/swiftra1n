#include "../jailbreak/UsbLiter8Adapter.hpp"

#include <iostream>
#include <vector>

using namespace swiftra1n::jailbreak;

int main() {
    UsbLiter8Adapter adapter;

    int passed = 0;
    int failed = 0;

    auto test = [&](const char* name,
                    const std::vector<UsbSerialPortInfo>& ports,
                    UsbLiter8Status expected,
                    bool identityExpected) {

        const auto result = adapter.assess(ports);

        const bool correctStatus =
            result.status == expected;

        const bool correctIdentity =
            result.boardIdentityVerified == identityExpected;

        const bool safelyBlocked = !result.canLaunch;

        const bool success =
            correctStatus && correctIdentity && safelyBlocked;

        std::cout << (success ? "[PASS] " : "[FAIL] ")
                  << name << '\n';

        std::cout << "       Status: "
                  << usbLiter8StatusName(result.status) << '\n';

        std::cout << "       Identity verified: "
                  << (result.boardIdentityVerified ? "YES" : "NO")
                  << '\n';

        std::cout << "       Can launch: "
                  << (result.canLaunch ? "YES" : "NO")
                  << "\n\n";

        success ? ++passed : ++failed;
    };

    test(
        "No USB serial devices",
        {},
        UsbLiter8Status::boardNotFound,
        false
    );

    UsbSerialPortInfo genericPort;
    genericPort.devicePath = "/dev/ttyACM0";

    test(
        "Generic serial interface",
        {genericPort},
        UsbLiter8Status::serialCandidateFound,
        false
    );

    UsbSerialPortInfo rp2350Candidate;
    rp2350Candidate.devicePath = "/dev/ttyACM1";
    rp2350Candidate.vendorId = "2e8a";
    rp2350Candidate.productId = "000a";
    rp2350Candidate.manufacturer = "Raspberry Pi";
    rp2350Candidate.product = "Pico 2";

    test(
        "RP2350-like descriptor remains unverified",
        {rp2350Candidate},
        UsbLiter8Status::serialCandidateFound,
        false
    );

    std::cout << "====================================\n";
    std::cout << "USB Liter8 Adapter Tests\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "====================================\n";

    return failed == 0 ? 0 : 1;
}
