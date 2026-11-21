#include "Compatibility.hpp"
#include "profiles/FirmwareProfiles.hpp"

#include <charconv>
#include <string_view>
#include <vector>
#include <system_error>

namespace swiftra1n::jailbreak {

namespace {

bool isKnownChip(const std::string& chip) {
    return chip == "A7"  ||
           chip == "A8"  ||
           chip == "A8X" ||
           chip == "A9"  ||
           chip == "A9X" ||
           chip == "A10" ||
           chip == "A10X" ||
           chip == "A11" ||
           chip == "A12" ||
           chip == "A12X" ||
           chip == "A12Z" ||
           chip == "A13" ||
           chip == "A14" ||
           chip == "A15" ||
           chip == "A16" ||
           chip == "A17" ||
           chip == "A18" ||
           chip == "A19";
}

bool isTargetChip(const std::string& chip) {
    return chip == "A12"  ||
           chip == "A12X" ||
           chip == "A12Z" ||
           chip == "A13";
}

bool parseVersion(
    std::string_view version,
    std::vector<unsigned long long>& parts
) {
    parts.clear();

    if (version.empty()) {
        return false;
    }

    while (!version.empty()) {
        const auto separator = version.find('.');
        const auto component = version.substr(0, separator);

        if (component.empty()) {
            return false;
        }

        unsigned long long value = 0;

        const auto result = std::from_chars(
            component.data(),
            component.data() + component.size(),
            value
        );

        if (result.ec != std::errc{} ||
            result.ptr != component.data() + component.size()) {
            return false;
        }

        parts.push_back(value);

        if (separator == std::string_view::npos) {
            break;
        }

        version.remove_prefix(separator + 1);

        if (version.empty()) {
            return false;
        }
    }

    return true;
}

int compareVersions(
    const std::vector<unsigned long long>& left,
    const std::vector<unsigned long long>& right
) {
    const auto count = left.size() > right.size()
        ? left.size()
        : right.size();

    for (std::size_t i = 0; i < count; ++i) {
        const auto a = i < left.size() ? left[i] : 0;
        const auto b = i < right.size() ? right[i] : 0;

        if (a < b) return -1;
        if (a > b) return 1;
    }

    return 0;
}

CompatibilityResult makeResult(
    CompatibilityStatus status,
    const std::string& reason
) {
    return {
        status,
        reason,
        "",
        ""
    };
}

}

CompatibilityResult CompatibilityChecker::check(
    const devices::DeviceInfo& device
) const {

    if (device.productType.empty() ||
        device.productType == "Unknown") {
        return makeResult(
            CompatibilityStatus::untested,
            "Unknown device model."
        );
    }

    if (device.chip.empty() ||
        device.chip == "Unknown" ||
        !isKnownChip(device.chip)) {
        return makeResult(
            CompatibilityStatus::untested,
            "Chip identification is not confirmed."
        );
    }

    if (!isTargetChip(device.chip)) {
        return makeResult(
            CompatibilityStatus::unsupported,
            "Device is outside the current SwiftRa1n target."
        );
    }

    const auto* hardware = findDeviceProfile(device);

    if (hardware == nullptr) {
        return makeResult(
            CompatibilityStatus::untested,
            "No exact hardware profile exists for this model and chip."
        );
    }

    if (device.osVersion.empty() ||
        device.osVersion == "Unknown") {
        return makeResult(
            CompatibilityStatus::untested,
            "iOS version is unavailable."
        );
    }

    std::vector<unsigned long long> installedVersion;
    std::vector<unsigned long long> firstVersion;

    if (!parseVersion(device.osVersion, installedVersion) ||
        !parseVersion(hardware->firstOSVersion, firstVersion)) {
        return makeResult(
            CompatibilityStatus::untested,
            "Invalid or unavailable OS version metadata."
        );
    }

    if (compareVersions(installedVersion, firstVersion) < 0) {
        return makeResult(
            CompatibilityStatus::untested,
            "Installed OS version predates the recorded device release version."
        );
    }

    if (device.buildVersion.empty() ||
        device.buildVersion == "Unknown") {
        return makeResult(
            CompatibilityStatus::untested,
            "Build ID is unavailable."
        );
    }

    const auto* firmware = findFirmwareProfile(device);

    if (firmware == nullptr) {
        return makeResult(
            CompatibilityStatus::untested,
            "No exact firmware record exists. Compatibility is not verified."
        );
    }

    switch (firmware->evidence) {

        case ProfileEvidence::researchOnly:
            return makeResult(
                CompatibilityStatus::researchOnly,
                std::string(firmware->description)
            );

        case ProfileEvidence::independentlyVerified:
            return makeResult(
                CompatibilityStatus::independentlyVerified,
                std::string(firmware->description)
            );

        case ProfileEvidence::knownUnsupported:
            return makeResult(
                CompatibilityStatus::unsupported,
                std::string(firmware->description)
            );
    }

    return makeResult(
        CompatibilityStatus::untested,
        "Unknown firmware evidence classification."
    );
}

const char* statusName(CompatibilityStatus status) {
    switch (status) {

        case CompatibilityStatus::locallyTested:
            return "Locally tested";

        case CompatibilityStatus::independentlyVerified:
            return "Independently verified";

        case CompatibilityStatus::researchOnly:
            return "Research only";

        case CompatibilityStatus::unsupported:
            return "Unsupported";

        case CompatibilityStatus::untested:
            return "Untested";
    }

    return "Unknown";
}

}
