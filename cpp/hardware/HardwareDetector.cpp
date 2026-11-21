
#include "HardwareDetector.hpp"

#include <libusb.h>

#include <iomanip>
#include <sstream>

namespace swiftra1n::hardware {

namespace {

// ==========================================
// USB String Descriptors
// ==========================================

std::string readUsbString(
    libusb_device_handle* handle,
    std::uint8_t descriptorIndex
) {
    if (descriptorIndex == 0) {
        return "Not provided";
    }

    unsigned char buffer[256]{};

    const int length = libusb_get_string_descriptor_ascii(
        handle,
        descriptorIndex,
        buffer,
        sizeof(buffer)
    );

    if (length < 0) {
        return "Unavailable";
    }

    return std::string(
        reinterpret_cast<const char*>(buffer),
        static_cast<std::size_t>(length)
    );
}

// ==========================================
// USB Device Information
// ==========================================

UsbDeviceInfo readDevice(libusb_device* device) {
    UsbDeviceInfo info;

    libusb_device_descriptor descriptor{};

    const int result = libusb_get_device_descriptor(
        device,
        &descriptor
    );

    if (result != LIBUSB_SUCCESS) {
        throw result;
    }

    info.busNumber = libusb_get_bus_number(device);
    info.address = libusb_get_device_address(device);

    info.vendorId = descriptor.idVendor;
    info.productId = descriptor.idProduct;

    info.deviceClass = descriptor.bDeviceClass;
    info.deviceSubClass = descriptor.bDeviceSubClass;
    info.deviceProtocol = descriptor.bDeviceProtocol;

    info.configurations = descriptor.bNumConfigurations;
    info.speed = libusb_get_device_speed(device);

    // USB topology: bus-port.port
    std::uint8_t ports[8]{};

    const int portCount = libusb_get_port_numbers(
        device,
        ports,
        sizeof(ports)
    );

    if (portCount > 0) {
        info.portPath.assign(ports, ports + portCount);
    }

    // Open only to read standard manufacturer/product
    // descriptors. No interfaces are claimed and no
    // device commands, resets or firmware operations occur.

    libusb_device_handle* handle = nullptr;

    const int openResult = libusb_open(device, &handle);

    if (openResult != LIBUSB_SUCCESS) {
        info.accessError = libusb_error_name(openResult);
        return info;
    }

    info.stringsAccessible = true;

    info.manufacturer = readUsbString(
        handle,
        descriptor.iManufacturer
    );

    info.product = readUsbString(
        handle,
        descriptor.iProduct
    );

    // Deliberately do not read descriptor.iSerialNumber.

    libusb_close(handle);

    return info;
}

} // namespace

// ==========================================
// Generic USB Scanner
// ==========================================

UsbScanResult HardwareDetector::scan() const {
    UsbScanResult result;

    libusb_context* context = nullptr;

    const int initResult = libusb_init(&context);

    if (initResult != LIBUSB_SUCCESS) {
        result.error =
            std::string("libusb initialization failed: ") +
            libusb_error_name(initResult);

        return result;
    }

    libusb_device** deviceList = nullptr;

    const ssize_t count = libusb_get_device_list(
        context,
        &deviceList
    );

    if (count < 0) {
        result.error =
            std::string("USB enumeration failed: ") +
            libusb_error_name(static_cast<int>(count));

        libusb_exit(context);
        return result;
    }

    for (ssize_t index = 0; index < count; ++index) {
        try {
            result.devices.push_back(
                readDevice(deviceList[index])
            );
        } catch (int) {
            ++result.descriptorErrors;
        }
    }

    libusb_free_device_list(deviceList, 1);
    libusb_exit(context);

    result.success = true;

    return result;
}

// ==========================================
// Formatting
// ==========================================

std::string formatUsbId(std::uint16_t value) {
    std::ostringstream stream;

    stream << std::hex
           << std::uppercase
           << std::setfill('0')
           << std::setw(4)
           << value;

    return stream.str();
}

std::string usbSpeedName(int speed) {
    switch (speed) {
        case LIBUSB_SPEED_LOW:
            return "Low Speed";

        case LIBUSB_SPEED_FULL:
            return "Full Speed";

        case LIBUSB_SPEED_HIGH:
            return "High Speed";

        case LIBUSB_SPEED_SUPER:
            return "SuperSpeed";

        case LIBUSB_SPEED_SUPER_PLUS:
            return "SuperSpeed+";

        default:
            return "Unknown";
    }
}

} // namespace swiftra1n::hardware
