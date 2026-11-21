
#include "../jailbreak/ExploitBackend.hpp"

#include <iostream>
#include <string>

using namespace swiftra1n::jailbreak;

int main() {
    ExploitBackendAdapter adapter;

    int passed = 0;
    int failed = 0;

    auto test = [&](const std::string& name,
                    const BackendEnvironment& environment,
                    BackendStatus expected) {

        const auto result = adapter.inspectLiter8(environment);

        const bool correctStatus = result.status == expected;
        const bool safelyBlocked = !result.canLaunch;

        const bool success = correctStatus && safelyBlocked;

        std::cout << (success ? "[PASS] " : "[FAIL] ")
                  << name << '\n';

        std::cout << "       Status: "
                  << backendStatusName(result.status) << '\n';

        std::cout << "       Launch: "
                  << (result.canLaunch ? "YES" : "NO") << "\n\n";

        success ? ++passed : ++failed;
    };

    BackendEnvironment linuxHost;
    linuxHost.platform = HostPlatform::linux;

    test(
        "Linux host is rejected for Liter8",
        linuxHost,
        BackendStatus::unsupportedHost
    );

    BackendEnvironment missingTools;
    missingTools.platform = HostPlatform::macOS;

    test(
        "Missing host prerequisites",
        missingTools,
        BackendStatus::missingPrerequisites
    );

    BackendEnvironment missingBinary;
    missingBinary.platform = HostPlatform::macOS;
    missingBinary.swiftCompilerFound = true;
    missingBinary.swift6Verified = true;
    missingBinary.macOS14OrNewer = true;
    missingBinary.xcodeCLTFound = true;
    missingBinary.homebrewFound = true;

    test(
        "Missing Liter8 executable",
        missingBinary,
        BackendStatus::missingExecutable
    );

    BackendEnvironment missingBoard = missingBinary;
    missingBoard.executableFound = true;

    test(
        "RP2350 hardware not verified",
        missingBoard,
        BackendStatus::hardwareNotVerified
    );

    BackendEnvironment allRequirements = missingBoard;
    allRequirements.rp2350Detected = true;

    test(
        "Requirements present, inspection only",
        allRequirements,
        BackendStatus::inspectionOnly
    );

    std::cout << "====================================\n";
    std::cout << "Backend Adapter Tests\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "====================================\n";

    return failed == 0 ? 0 : 1;
}
