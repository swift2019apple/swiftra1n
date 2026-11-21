#include <iostream>
#include <string>
#include <cstdlib>

#include "devices/DeviceManager.hpp"
#include "jailbreak/ExploitCore.hpp"

namespace {

constexpr const char* ORANGE = "\033[38;5;208m";
constexpr const char* WHITE  = "\033[97m";
constexpr const char* GREY   = "\033[90m";
constexpr const char* RED    = "\033[91m";
constexpr const char* GREEN  = "\033[92m";
constexpr const char* RESET  = "\033[0m";

void line() {
    std::cout << ORANGE
              << "+------------------------------------------------+\n"
              << RESET;
}

void showBanner() {
    std::cout << '\n';
    line();

    std::cout << ORANGE
              << "|                                                |\n"
              << "|                  SwiftRa1n                     |\n"
              << "|                                                |\n"
              << "|          VERSION 0.0.1  |  A12 - A13           |\n"
              << "|                                                |\n"
              << RESET;

    line();
    std::cout << '\n';
}

void pauseScreen() {
    std::cout << '\n' << GREY
              << "Press Enter to continue..."
              << RESET;

    std::string input;
    std::getline(std::cin, input);
}

void showUsbStatistics(
    const swiftra1n::devices::DeviceManagerResult& result
) {
    std::cout << ORANGE
              << "\n========== USB SCANNER ==========\n"
              << RESET;

    if (result.usbScanAvailable) {
        std::cout << GREEN << "Scanner: AVAILABLE\n" << RESET;
    } else {
        std::cout << RED << "Scanner: UNAVAILABLE\n" << RESET;

        if (!result.usbScanError.empty()) {
            std::cout << GREY
                      << "Error: " << result.usbScanError
                      << RESET << '\n';
        }
    }

    std::cout << WHITE
              << "USB devices: " << result.usbDeviceCount
              << "\nApple USB devices: " << result.appleUsbDeviceCount
              << "\nUSB hubs: " << result.usbHubCount
              << "\nKnown boards: " << result.knownBoardCount
              << "\nUnknown devices: " << result.unknownDeviceCount
              << "\nDescriptor errors: " << result.descriptorErrors
              << '\n'
              << RESET;
}

void showLogicalDevices(
    const std::vector<swiftra1n::devices::DeviceInfo>& devices
) {
    std::cout << ORANGE
              << "\n========== APPLE DEVICES ==========\n"
              << RESET;

    if (devices.empty()) {
        std::cout << RED
                  << "No logical Apple devices detected.\n"
                  << RESET;

        std::cout << GREY
                  << "A physical USB connection may still be visible.\n"
                  << RESET;

        return;
    }

    for (std::size_t i = 0; i < devices.size(); ++i) {
        const auto& device = devices[i];

        std::cout << ORANGE
                  << "\nDEVICE #" << i + 1
                  << "\n----------------------------------------\n"
                  << RESET;

        std::cout << WHITE
                  << "Product: " << device.productType
                  << "\nFamily: " << device.family
                  << "\nIdentifier: " << device.identifier
                  << "\nChip: " << device.chip
                  << "\nCPID: " << device.cpid
                  << "\nBoard ID: " << device.boardID
                  << "\nECID: " << device.ecid
                  << "\niOS: " << device.osVersion
                  << "\nBuild: " << device.buildVersion
                  << "\nMode: " << device.mode
                  << '\n'
                  << RESET;
    }
}

void showDeviceInformation() {
    using namespace swiftra1n;

    std::cout << '\n' << ORANGE
              << "========== DEVICE INFORMATION ==========\n"
              << RESET;

    devices::DeviceManager manager;
    const auto result = manager.scan();

    showLogicalDevices(result.devices);

    std::cout << ORANGE
              << "\n========== SCAN SUMMARY ==========\n"
              << RESET;

    if (result.appleUsbDeviceCount > 0 &&
        result.devices.empty()) {
        std::cout << ORANGE
                  << "Apple USB hardware detected, but logical device "
                     "information is unavailable.\n"
                  << RESET;
    } else if (!result.devices.empty()) {
        std::cout << GREEN
                  << "Apple device information retrieved.\n"
                  << RESET;
    } else {
        std::cout << RED
                  << "No Apple device information available.\n"
                  << RESET;
    }
}

void showDeveloperInformation() {
    std::cout << '\n' << ORANGE
              << "========== DEVELOPER INFORMATION ==========\n"
              << RESET;

    std::cout << WHITE
              << "Developers: swift2019, euroo.\n"
              << "Telegram: @leijan203\n"
              << "Telegram channel: @leijan10\n"
              << "TikTok: @execc.c\n"
              << "Bug reports: @Leijan203\n"
              << "\nHello from the Swift team! This is our first project.\n"
              << RESET;
}

void jailbreakMenu() {
    using namespace swiftra1n;

    std::cout << '\n' << ORANGE
              << "============== JAILBREAK ==============\n"
              << RESET;

    devices::DeviceManager manager;
    const auto scan = manager.scan();

    if (scan.devices.empty()) {
        std::cout << RED
                  << "No Apple device detected.\n"
                  << RESET;
        return;
    }

    jailbreak::ExploitCore core;

    for (const auto& device : scan.devices) {
        const auto assessment = core.assess(device);

        std::cout << WHITE
                  << "\nDevice: " << device.productType
                  << "\nChip: " << device.chip
                  << "\niOS: " << device.osVersion
                  << "\nBuild: " << device.buildVersion
                  << "\nMode: " << device.mode
                  << RESET;

        std::cout << ORANGE
                  << "\n\nResearch profile: "
                  << jailbreak::coreStatusName(assessment.status)
                  << RESET;

        std::cout << GREY
                  << "\nDetails: " << assessment.reason
                  << RESET;

        std::cout << RED
                  << "\n\nJailbreak execution is not implemented."
                  << "\nNo changes have been made to your device.\n"
                  << RESET;
    }
}

void showMenu() {
    std::cout << WHITE
              << " [1] Jailbreak\n"
              << " [2] Device Information\n"
              << " [3] Developer Information\n"
              << " [4] Exit\n\n"
              << RESET;

    std::cout << ORANGE << "Select option: " << RESET;
}

} // namespace

int main() {
    while (true) {
        std::system("clear");

        showBanner();
        showMenu();

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            jailbreakMenu();
            pauseScreen();
        }
        else if (choice == "2") {
            showDeviceInformation();
            pauseScreen();
        }
        else if (choice == "3") {
            showDeveloperInformation();
            pauseScreen();
        }
        else if (choice == "4") {
            std::cout << GREEN << "\nGoodbye!\n" << RESET;
            break;
        }
        else {
            std::cout << RED << "\nInvalid option.\n" << RESET;
            pauseScreen();
        }
    }

    return 0;
}

