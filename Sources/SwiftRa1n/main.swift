л
import Foundation

let appName = "SwiftRa1n"
let version = "1.0.0"
let githubRepo = "YOUR_GITHUB_USERNAME/SwiftRa1n"

func banner() {
    print("""

    ╔══════════════════════════════════════════════╗
    ║                                              ║
    ║                 SwiftRa1n                    ║
    ║                                              ║
    ║          VERSION \(version) | A12 - A13      ║
    ╚══════════════════════════════════════════════╝

    """)
}

func runCommand(
    _ command: String,
    arguments: [String] = []
) -> String? {
    let process = Process()
    let output = Pipe()
    let errors = Pipe()

    process.executableURL = URL(fileURLWithPath: "/usr/bin/env")
    process.arguments = [command] + arguments
    process.standardOutput = output
    process.standardError = errors

    do {
        try process.run()
        let data = output.fileHandleForReading.readDataToEndOfFile()
        process.waitUntilExit()

        guard process.terminationStatus == 0 else {
            return nil
        }

        return String(
            data: data,
            encoding: .utf8
        )?.trimmingCharacters(in: .whitespacesAndNewlines)
    } catch {
        return nil
    }
}

// MARK: - System

func checkSudo() {
    print("[*] Checking administrator permissions...")

    guard let uid = runCommand("id", arguments: ["-u"]),
          uid == "0" else {
        print("[!] Administrator permissions required.")
        print("[*] Run SwiftRa1n using sudo.")
        return
    }

    print("[+] Administrator permissions confirmed.")
}

func checkDependencies() {
    print("\n[*] Checking dependencies...\n")

    let dependencies = [
        "idevice_id",
        "ideviceinfo",
        "irecovery",
        "git",
        "curl"
    ]

    for dependency in dependencies {
        if runCommand("which", arguments: [dependency]) != nil {
            print("[+] \(dependency): installed")
        } else {
            print("[-] \(dependency): missing")
        }
    }
}

// MARK: - Device Detection

func detectDevices() {
    print("\n[*] Searching for connected Apple devices...\n")

    guard let result = runCommand("idevice_id", arguments: ["-l"]) else {
        print("[!] idevice_id unavailable.")
        print("[*] Install libimobiledevice.")
        return
    }

    let devices = result
        .split(separator: "\n")
        .map(String.init)

    if devices.isEmpty {
        print("[!] No devices detected.")
        return
    }

    for udid in devices {
        print("\n[+] Device detected!")
        print("UDID: \(udid)")

        let model = runCommand(
            "ideviceinfo",
            arguments: ["-u", udid, "-k", "ProductType"]
        ) ?? "Unknown"

        let ios = runCommand(
            "ideviceinfo",
            arguments: ["-u", udid, "-k", "ProductVersion"]
        ) ?? "Unknown"

        print("Model: \(model)")
        print("iOS: \(ios)")
        print("Jailbreak: NOT VERIFIED")
        print("--------------------------------")
    }
}

// MARK: - Compatibility

func compatibilityInfo() {
    print("""

    SwiftRa1n Compatibility

    Device detection: Available
    A12 jailbreak: Not yet verified
    A13 jailbreak: Not yet verified

    No jailbreak operation will be performed
    without a verified exploit chain.

    """)
}

// MARK: - Developers

func developers() {
    print("""

    SwiftRa1n Developers

    Project: SwiftRa1n
    Version: \(version)
    Repository: \(githubRepo)

    """)
}

// MARK: - Menu

func mainMenu() {
    while true {
        banner()

        print("[1] Detect Apple devices")
        print("[2] Check dependencies")
        print("[3] Compatibility information")
        print("[4] Developers")
        print("[5] Exit")
        print("")
        print("SwiftRa1n > ", terminator: "")

        guard let choice = readLine() else {
            return
        }

        switch choice {
        case "1":
            detectDevices()

        case "2":
            checkDependencies()

        case "3":
            compatibilityInfo()

        case "4":
            developers()

        case "5":
            print("Goodbye!")
            return

        default:
            print("[!] Invalid option.")
        }

        print("\nPress Enter to continue...")
        _ = readLine()
    }
}

mainMenu()
