
CXX := g++
PKG_CONFIG := pkg-config

CPPFLAGS := -Icpp
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2

USB_AVAILABLE := $(shell $(PKG_CONFIG) --exists libusb-1.0 && echo yes)

ifeq ($(USB_AVAILABLE),)
$(error libusb-1.0 not found. Install it with: sudo apt install libusb-1.0-0-dev pkg-config)
endif

USB_CFLAGS := $(shell $(PKG_CONFIG) --cflags libusb-1.0)
USB_LIBS := $(shell $(PKG_CONFIG) --libs libusb-1.0)

BUILD := build
APP := $(BUILD)/swiftra1n

APP_SOURCES := \
	cpp/main.cpp \
	cpp/devices/DeviceDetector.cpp \
	cpp/devices/DeviceManager.cpp \
	cpp/hardware/HardwareDetector.cpp \
	cpp/hardware/HardwareProfileRegistry.cpp \
	cpp/jailbreak/Compatibility.cpp \
	cpp/jailbreak/ExploitEngine.cpp \
	cpp/jailbreak/ExploitCore.cpp

.PHONY: all test clean

all: $(APP)

$(APP): $(APP_SOURCES)
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(USB_CFLAGS) $(CXXFLAGS) $(APP_SOURCES) $(USB_LIBS) -o $@

$(BUILD)/compatibility_test: cpp/tests/compatibility_test.cpp cpp/jailbreak/Compatibility.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BUILD)/exploit_engine_test: cpp/tests/exploit_engine_test.cpp cpp/jailbreak/Compatibility.cpp cpp/jailbreak/ExploitEngine.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BUILD)/hardware_profile_test: cpp/tests/hardware_profile_test.cpp cpp/hardware/HardwareProfileRegistry.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BUILD)/hardware_detector_smoke: cpp/tests/hardware_detector_smoke.cpp cpp/hardware/HardwareDetector.cpp cpp/hardware/HardwareProfileRegistry.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(USB_CFLAGS) $(CXXFLAGS) $^ $(USB_LIBS) -o $@

$(BUILD)/exploit_core_test: cpp/tests/exploit_core_test.cpp cpp/jailbreak/ExploitCore.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(BUILD)/compatibility_test \
      $(BUILD)/exploit_engine_test \
      $(BUILD)/hardware_profile_test \
      $(BUILD)/hardware_detector_smoke \
      $(BUILD)/exploit_core_test
	$(BUILD)/compatibility_test
	$(BUILD)/exploit_engine_test
	$(BUILD)/hardware_profile_test
	$(BUILD)/hardware_detector_smoke
	$(BUILD)/exploit_core_test

clean:
	rm -f $(APP) \
		$(BUILD)/compatibility_test \
		$(BUILD)/exploit_engine_test \
		$(BUILD)/hardware_profile_test \
		$(BUILD)/hardware_detector_smoke \
		$(BUILD)/exploit_core_test
