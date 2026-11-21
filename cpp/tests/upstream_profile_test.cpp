
#include "../jailbreak/ExploitEngine.hpp"

#include <iostream>

int main() {
    using namespace swiftra1n;

    devices::DeviceInfo device;

    device.identifier = "MOCK-IPHONE-11";
    device.productType = "iPhone12,1";
    device.family = "iPhone";
    device.chip = "A13";
    device.osVersion = "27.2";
    device.buildVersion = "24B119";
    device.mode = "MOCK";

    jailbreak::CompatibilityChecker checker;
    jailbreak::ExploitEngine engine;

    const auto compatibility = checker.check(device);
    const auto assessment = engine.preflight(device);

    const bool profileFound =
        compatibility.status ==
        jailbreak::CompatibilityStatus::upstreamReported;

    const bool executionBlocked =
        assessment.state ==
            jailbreak::ExploitState::backendNotImplemented &&
        !assessment.canExecute;

    std::cout << "["
              << (profileFound ? "PASS" : "FAIL")
              << "] Exact Build ID profile\n";

    std::cout << "["
              << (executionBlocked ? "PASS" : "FAIL")
              << "] Exploit execution remains blocked\n";

    std::cout << "Status: "
              << jailbreak::statusName(compatibility.status)
              << '\n';

    std::cout << "Source: "
              << compatibility.sourceUrl
              << '\n';

    std::cout << "Engine: "
              << jailbreak::exploitStateName(assessment.state)
              << '\n';

    return profileFound && executionBlocked ? 0 : 1;
}
