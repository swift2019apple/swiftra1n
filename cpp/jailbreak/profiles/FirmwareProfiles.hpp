
#pragma once

#include "../../devices/DeviceDetector.hpp"

#include <array>
#include <string_view>

namespace swiftra1n::jailbreak {

enum class ProfileEvidence {
    researchOnly,
    independentlyVerified,
    knownUnsupported
};

struct DeviceProfile {
    std::string_view productType;
    std::string_view chip;
    std::string_view deviceFamily;
    std::string_view firstOSVersion;
};

struct FirmwareProfile {
    std::string_view productType;
    std::string_view chip;
    std::string_view iosVersion;
    std::string_view buildVersion;
    ProfileEvidence evidence;
    std::string_view description;
};

// Hardware database.
// firstOSVersion = earliest OS version shipped with the device.
// It does NOT establish jailbreak compatibility.

inline constexpr std::array<DeviceProfile, 26> deviceProfiles{{

    // A12 — iPhone
    {"iPhone11,2", "A12", "iPhone XS", "12.0"},
    {"iPhone11,4", "A12", "iPhone XS Max", "12.0"},
    {"iPhone11,6", "A12", "iPhone XS Max", "12.0"},
    {"iPhone11,8", "A12", "iPhone XR", "12.0.1"},

    // A12 — iPad
    {"iPad11,1", "A12", "iPad mini (5th generation)", "12.2"},
    {"iPad11,2", "A12", "iPad mini (5th generation)", "12.2"},
    {"iPad11,3", "A12", "iPad Air (3rd generation)", "12.2"},
    {"iPad11,4", "A12", "iPad Air (3rd generation)", "12.2"},

    // A12X — iPad Pro 2018
    {"iPad8,1", "A12X", "iPad Pro 11-inch", "12.1"},
    {"iPad8,2", "A12X", "iPad Pro 11-inch", "12.1"},
    {"iPad8,3", "A12X", "iPad Pro 11-inch", "12.1"},
    {"iPad8,4", "A12X", "iPad Pro 11-inch", "12.1"},
    {"iPad8,5", "A12X", "iPad Pro 12.9-inch", "12.1"},
    {"iPad8,6", "A12X", "iPad Pro 12.9-inch", "12.1"},
    {"iPad8,7", "A12X", "iPad Pro 12.9-inch", "12.1"},
    {"iPad8,8", "A12X", "iPad Pro 12.9-inch", "12.1"},

    // A12Z — iPad Pro 2020
    {"iPad8,9", "A12Z", "iPad Pro 11-inch", "13.4"},
    {"iPad8,10", "A12Z", "iPad Pro 11-inch", "13.4"},
    {"iPad8,11", "A12Z", "iPad Pro 12.9-inch", "13.4"},
    {"iPad8,12", "A12Z", "iPad Pro 12.9-inch", "13.4"},

    // A13 — iPhone
    {"iPhone12,1", "A13", "iPhone 11", "13.0"},
    {"iPhone12,3", "A13", "iPhone 11 Pro", "13.0"},
    {"iPhone12,5", "A13", "iPhone 11 Pro Max", "13.0"},
    {"iPhone12,8", "A13", "iPhone SE (2nd generation)", "13.4"},

    // A13 — iPad
    {"iPad12,1", "A13", "iPad (9th generation)", "15.0"},
    {"iPad12,2", "A13", "iPad (9th generation)", "15.0"}
}};

// Exact firmware records.
// A record is not proof of jailbreak compatibility.

inline constexpr std::array<FirmwareProfile, 1> firmwareProfiles{{

    {
        "iPhone12,1",
        "A13",
        "27.2",
        "24B119",
        ProfileEvidence::researchOnly,
        "Research profile. Jailbreak compatibility is not verified."
    }

}};

inline const DeviceProfile* findDeviceProfile(
    const devices::DeviceInfo& device
) noexcept {

    for (const auto& profile : deviceProfiles) {
        if (device.productType == profile.productType &&
            device.chip == profile.chip) {
            return &profile;
        }
    }

    return nullptr;
}

inline const FirmwareProfile* findFirmwareProfile(
    const devices::DeviceInfo& device
) noexcept {

    for (const auto& profile : firmwareProfiles) {
        if (device.productType == profile.productType &&
            device.chip == profile.chip &&
            device.osVersion == profile.iosVersion &&
            device.buildVersion == profile.buildVersion) {
            return &profile;
        }
    }

    return nullptr;
}

} // namespace swiftra1n::jailbreak
