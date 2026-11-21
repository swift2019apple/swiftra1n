
#pragma once

#include <string>
#include <vector>

namespace swiftra1n::devices {

struct DeviceInfo {
    std::string identifier;
    std::string productType = "Unknown";
    std::string family = "Unknown";
    std::string chip = "Unknown";
    std::string cpid = "Unknown";
    std::string boardID = "Unknown";
    std::string ecid = "Unknown";
    std::string osVersion = "Unknown";
    std::string buildVersion = "Unknown";
    std::string mode = "Unknown";
};

class DeviceDetector {
public:
    std::vector<DeviceInfo> detect() const;
};

}
