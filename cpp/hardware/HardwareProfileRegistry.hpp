
#pragma once

#include "HardwareDetector.hpp"

#include <string>

namespace swiftra1n::hardware {

enum class HardwareCategory {
    appleDevice,
    usbHub,
    knownBoard,
    unknown
};

enum class MatchBasis {
    vendorAndDescriptor,
    vendorOnly,
    usbDeviceClass,
    none
};

struct HardwareClassification {
    HardwareCategory category = HardwareCategory::unknown;
    MatchBasis matchBasis = MatchBasis::none;

    std::string name = "Unknown USB device";
    std::string reason;

    bool recognized = false;
};

class HardwareProfileRegistry {
public:
    HardwareClassification classify(
        const UsbDeviceInfo& device
    ) const;
};

const char* hardwareCategoryName(HardwareCategory category);
const char* matchBasisName(MatchBasis basis);

}
