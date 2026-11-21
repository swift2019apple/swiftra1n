#pragma once

#include "../devices/DeviceDetector.hpp"

#include <string>

namespace swiftra1n::jailbreak {

enum class CompatibilityStatus {
    locallyTested,
    independentlyVerified,
    researchOnly,
    unsupported,
    untested
};

struct CompatibilityResult {
    CompatibilityStatus status;
    std::string reason;
    std::string exploitName;
    std::string sourceUrl;
};

class CompatibilityChecker {
public:
    CompatibilityResult check(
        const devices::DeviceInfo& device
    ) const;
};

const char* statusName(CompatibilityStatus status);

}
