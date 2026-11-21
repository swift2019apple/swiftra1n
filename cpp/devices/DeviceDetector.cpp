
#include "DeviceDetector.hpp"

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <fcntl.h>
#include <optional>
#include <sstream>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace swiftra1n::devices {

namespace {

// ==========================================
// String Helpers
// ==========================================

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    const auto last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

std::string getField(
    const std::string& source,
    const std::string& key
) {
    const auto position = source.find(key);

    if (position == std::string::npos) {
        return "Unknown";
    }

    const auto start = position + key.size();
    const auto end = source.find_first_of("\r\n", start);

    return trim(source.substr(
        start,
        end == std::string::npos
            ? std::string::npos
            : end - start
    ));
}

// ==========================================
// Process Runner
// ==========================================

std::optional<std::string> run(
    const std::vector<std::string>& arguments
) {
    if (arguments.empty()) {
        return std::nullopt;
    }

    int pipefd[2];

    if (pipe(pipefd) == -1) {
        return std::nullopt;
    }

    const pid_t pid = fork();

    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return std::nullopt;
    }

    if (pid == 0) {
        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            _exit(127);
        }

        close(pipefd[1]);

        const int nullfd = open("/dev/null", O_WRONLY);

        if (nullfd >= 0) {
            dup2(nullfd, STDERR_FILENO);
            close(nullfd);
        }

        std::vector<char*> argv;

        for (const auto& argument : arguments) {
            argv.push_back(
                const_cast<char*>(argument.c_str())
            );
        }

        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        _exit(127);
    }

    close(pipefd[1]);

    std::string output;
    char buffer[4096];
    bool readError = false;

    while (true) {
        const ssize_t count = read(
            pipefd[0],
            buffer,
            sizeof(buffer)
        );

        if (count > 0) {
            output.append(
                buffer,
                static_cast<std::size_t>(count)
            );
        } else if (count == 0) {
            break;
        } else if (errno != EINTR) {
            readError = true;
            break;
        }
    }

    close(pipefd[0]);

    int status = 0;
    pid_t waited;

    do {
        waited = waitpid(pid, &status, 0);
    } while (waited == -1 && errno == EINTR);

    if (readError ||
        waited == -1 ||
        !WIFEXITED(status) ||
        WEXITSTATUS(status) != 0) {
        return std::nullopt;
    }

    return trim(output);
}

// ==========================================
// Device Identification
// ==========================================

std::string familyFromModel(const std::string& model) {
    if (model.starts_with("iPhone")) {
        return "iPhone";
    }

    if (model.starts_with("iPad")) {
        return "iPad";
    }

    if (model == "Unknown") {
        return "Unknown";
    }

    return "Other Apple device";
}

std::string chipFromCPID(std::string cpid) {
    cpid = trim(cpid);

    std::transform(
        cpid.begin(),
        cpid.end(),
        cpid.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    if (cpid.starts_with("0x")) {
        cpid.erase(0, 2);
    }

    if (cpid == "8020") return "A12";
    if (cpid == "8027") return "A12X/A12Z";
    if (cpid == "8030") return "A13";

    return "Unknown";
}

// Exact model identifiers only.
// Unknown devices are deliberately not guessed.

std::string chipFromModel(const std::string& model) {
    static const std::vector<std::string> a12 = {
        "iPhone11,2",
        "iPhone11,4",
        "iPhone11,6",
        "iPhone11,8",
        "iPad11,1",
        "iPad11,2",
        "iPad11,3",
        "iPad11,4",
        "iPad11,6",
        "iPad11,7"
    };

    static const std::vector<std::string> a12x = {
        "iPad8,1",
        "iPad8,2",
        "iPad8,3",
        "iPad8,4",
        "iPad8,5",
        "iPad8,6",
        "iPad8,7",
        "iPad8,8"
    };

    static const std::vector<std::string> a12z = {
        "iPad8,9",
        "iPad8,10",
        "iPad8,11",
        "iPad8,12"
    };

    static const std::vector<std::string> a13 = {
        "iPhone12,1",
        "iPhone12,3",
        "iPhone12,5",
        "iPhone12,8",
        "iPad12,1",
        "iPad12,2"
    };

    if (std::find(a12.begin(), a12.end(), model) != a12.end()) {
        return "A12";
    }

    if (std::find(a12x.begin(), a12x.end(), model) != a12x.end()) {
        return "A12X";
    }

    if (std::find(a12z.begin(), a12z.end(), model) != a12z.end()) {
        return "A12Z";
    }

    if (std::find(a13.begin(), a13.end(), model) != a13.end()) {
        return "A13";
    }

    return "Unknown";
}

// ==========================================
// Normal Mode
// ==========================================

std::string getDeviceValue(
    const std::string& udid,
    const std::string& key
) {
    auto result = run({
        "ideviceinfo",
        "-u", udid,
        "-k", key
    });

    return result.value_or("Unknown");
}

DeviceInfo readNormalDevice(const std::string& udid) {
    DeviceInfo device;

    device.identifier = udid;
    device.productType = getDeviceValue(udid, "ProductType");
    device.osVersion = getDeviceValue(udid, "ProductVersion");
    device.buildVersion = getDeviceValue(udid, "BuildVersion");
    device.ecid = getDeviceValue(udid, "UniqueChipID");

    device.family = familyFromModel(device.productType);
    device.chip = chipFromModel(device.productType);
    device.mode = "Normal";

    return device;
}

// ==========================================
// Recovery / DFU
// ==========================================

std::optional<DeviceInfo> readRecoveryDevice() {
    auto result = run({
        "irecovery",
        "-q"
    });

    if (!result || result->empty()) {
        return std::nullopt;
    }

    DeviceInfo device;

    device.cpid = getField(*result, "CPID:");
    device.ecid = getField(*result, "ECID:");
    device.boardID = getField(*result, "BDID:");

    device.productType = getField(*result, "PRODUCT:");
    device.family = familyFromModel(device.productType);

    device.chip = chipFromCPID(device.cpid);
    device.osVersion = "Unavailable";

    const std::string reportedMode = getField(*result, "MODE:");

    if (reportedMode == "DFU") {
        device.mode = "DFU";
    } else if (reportedMode == "Recovery") {
        device.mode = "Recovery";
    } else {
        device.mode = "Recovery/DFU (not distinguished)";
    }

    if (device.ecid != "Unknown") {
        device.identifier = "ECID: " + device.ecid;
    } else {
        device.identifier = "Recovery/DFU device";
    }

    return device;
}

} // namespace

// ==========================================
// Public API
// ==========================================

std::vector<DeviceInfo> DeviceDetector::detect() const {
    std::vector<DeviceInfo> devices;

    // Find all devices connected through USB.
    auto result = run({
        "idevice_id",
        "-l"
    });

    if (result) {
        std::istringstream stream(*result);
        std::string udid;

        while (std::getline(stream, udid)) {
            udid = trim(udid);

            if (udid.empty()) {
                continue;
            }

            devices.push_back(readNormalDevice(udid));
        }
    }

    // Check whether a device is visible through iRecovery.
    auto recovery = readRecoveryDevice();

    if (recovery) {
        devices.push_back(*recovery);
    }

    return devices;
}

} // namespace swiftra1n::devices

