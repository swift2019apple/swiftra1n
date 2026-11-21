
#include "HardwareProfileRegistry.hpp"

#include <algorithm>
#include <cctype>

namespace swiftra1n::hardware {

namespace {

constexpr std::uint16_t APPLE_VID = 0x05AC;
constexpr std::uint8_t USB_HUB_CLASS = 0x09;

std::string lowercase(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char character) {
            return static_cast<char>(
                std::tolower(character)
            );
        }
    );

    return value;
}

bool contains(
    const std::string& value,
    const std::string& part
) {
    return lowercase(value).find(lowercase(part))
        != std::string::npos;
}

} // namespace

// ==========================================
// Hardware Classification
// ==========================================

HardwareClassification HardwareProfileRegistry::classify(
    const UsbDeviceInfo& device
) const {

    // USB hubs are identified by the device class.
    if (device.deviceClass == USB_HUB_CLASS) {
        return {
            HardwareCategory::usbHub,
            MatchBasis::usbDeviceClass,
            "USB Hub",
            "USB device class indicates a hub.",
            true
        };
    }

    // Apple vendor identification.
    if (device.vendorId == APPLE_VID) {

        if (contains(device.product, "iphone")) {
            return {
                HardwareCategory::appleDevice,
                MatchBasis::vendorAndDescriptor,
                "Apple iPhone",
                "Apple VID and product descriptor match.",
                true
            };
        }

        if (contains(device.product, "ipad")) {
            return {
                HardwareCategory::appleDevice,
                MatchBasis::vendorAndDescriptor,
                "Apple iPad",
                "Apple VID and product descriptor match.",
                true
            };
        }

        return {
            HardwareCategory::appleDevice,
            MatchBasis::vendorOnly,
            "Apple USB device",
            "Apple vendor ID found. Exact product is not established.",
            true
        };
    }

    // Unknown devices are deliberately not guessed.
    return {
        HardwareCategory::unknown,
        MatchBasis::none,
        "Unknown USB device",
        "No registered hardware profile matches this device.",
        false
    };
}

// ==========================================
// Category Names
// ==========================================

const char* hardwareCategoryName(HardwareCategory category) {
    switch (category) {
        case HardwareCategory::appleDevice:
            return "APPLE_DEVICE";

        case HardwareCategory::usbHub:
            return "USB_HUB";

        case HardwareCategory::knownBoard:
            return "KNOWN_BOARD";

        case HardwareCategory::unknown:
            return "UNKNOWN";
    }

    return "UNKNOWN";
}

// ==========================================
// Match Basis Names
// ==========================================

const char* matchBasisName(MatchBasis basis) {
    switch (basis) {
        case MatchBasis::vendorAndDescriptor:
            return "VENDOR_AND_DESCRIPTOR";

        case MatchBasis::vendorOnly:
            return "VENDOR_ONLY";

        case MatchBasis::usbDeviceClass:
            return "USB_DEVICE_CLASS";

        case MatchBasis::none:
            return "NONE";
    }

    return "NONE";
}

} // namespace swiftra1n::hardware
