
import os
import sys
import time
import subprocess

# ==============================
# SwiftRa1n Configuration
# ==============================

APP_NAME = "SwiftRa1n"
VERSION = "1.0.0"

ORANGE = "\033[38;5;208m"
WHITE = "\033[97m"
GRAY = "\033[90m"
RED = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"
BOLD = "\033[1m"


# ==============================
# Terminal
# ==============================

def clear():
    os.system("clear")



def banner():
    print(f"{ORANGE}{BOLD}")
    print("╔══════════════════════════════════════════════════╗")
    print("║                                                  ║")
    print("║                  SwiftRa1n                      ║")
    print("║                                                  ║")
    print("║                                                  ║")
    print("║          VERSION 1.0.0  |  A12 - A13             ║")
    print("╚══════════════════════════════════════════════════╝")
    print(RESET)

def device_info():
    def get_info(key):
        try:
            result = subprocess.run(
                ["ideviceinfo", "-k", key],
                capture_output=True,
                text=True,
                timeout=3
            )
            if result.returncode == 0:
                return result.stdout.strip()
        except (FileNotFoundError, subprocess.TimeoutExpired):
            pass
        return "Unknown"

    def recovery_info():
        try:
            result = subprocess.run(
                ["irecovery", "-q"],
                capture_output=True,
                text=True,
                timeout=3
            )
            if result.returncode == 0:
                return result.stdout
        except (FileNotFoundError, subprocess.TimeoutExpired):
            pass
        return ""

    ios = get_info("ProductVersion")
    model = get_info("ProductType")
    ecid = get_info("UniqueChipID")

    recovery = recovery_info()

    cpid = "Unknown"
    if recovery:
        for line in recovery.splitlines():
            if "CPID:" in line:
                cpid = line.split("CPID:", 1)[1].strip()
            elif "ECID:" in line:
                ecid = line.split("ECID:", 1)[1].strip()

    chip = "Unknown"
    if cpid.lower().replace("0x", "") == "8020":
        chip = "A12"
    elif cpid.lower().replace("0x", "") == "8030":
        chip = "A13"
    elif cpid.lower().replace("0x", "") == "8027":
        chip = "A12X"

    if recovery:
        mode = "Recovery / DFU"
    elif model != "Unknown":
        mode = "Normal"
    else:
        mode = "Not connected"

    print(f"{ORANGE}{BOLD}")
    print("╔══════════════════════════════════════════════════╗")
    print("║                  DEVICE INFORMATION              ║")
    print("╠══════════════════════════════════════════════════╣")
    print(RESET)

    print(f" {ORANGE}Device:{RESET}  {WHITE}{model}")
    print(f" {ORANGE}Chip:{RESET}    {WHITE}{chip}")
    print(f" {ORANGE}CPID:{RESET}    {WHITE}{cpid}")
    print(f" {ORANGE}ECID:{RESET}    {WHITE}{ecid}")
    print(f" {ORANGE}iOS:{RESET}     {WHITE}{ios}")
    print(f" {ORANGE}Mode:{RESET}    {WHITE}{mode}")

    print(f"{ORANGE}{BOLD}")
    print("╚══════════════════════════════════════════════════╝")
    print(RESET)

# ==============================
# SUDO
# ==============================

def check_sudo():
    print(f"{ORANGE}[!] Requesting sudo password...{RESET}")

    result = subprocess.run(["sudo", "-v"])

    if result.returncode != 0:
        print(f"{RED}[!] Sudo authentication failed!{RESET}")
        sys.exit(1)

    print(f"{GREEN}[+] Sudo authentication successful!{RESET}")
    time.sleep(1)


# ==============================
# MAIN MENU
# ==============================

def main_menu():

    while True:
        clear()
        banner()
        device_info()

        print(f"{WHITE}[1]{RESET} Install SwiftRa1n")
        print(f"{WHITE}[2]{RESET} Developer information")
        print(f"{WHITE}[3]{RESET} Exit")

        print()
        choice = input(f"{ORANGE}SwiftRa1n > {RESET}")

        if choice == "1":
            install_swiftrain()

        elif choice == "2":
            developers()

        elif choice == "3":
            print(f"\n{ORANGE}Goodbye!{RESET}")
            sys.exit(0)

        else:
            print(f"{RED}[!] Invalid option!{RESET}")
            time.sleep(1)


# ==============================
# INSTALLATION
# ==============================

def install_swiftrain():
    clear()
    banner()

    print(f"{ORANGE}[+] SwiftRa1n installation{RESET}\n")

    print("[*] Device detection module is not implemented yet.")
    print("[*] USB-Liter8 integration will be added next.")

    input("\nPress Enter to return...")


# ==============================
# DEVELOPERS
# ==============================

def developers():
    clear()
    banner()

    print(f"{ORANGE}========== DEVELOPERS =========={RESET}\n")

    print(f"{WHITE}swift2019{RESET}")
    print("Telegram: @leijan203")
    print("Telegram channel: @leijan10\n")

    print(f"{WHITE}euroo.{RESET}")
    print("TikTok: @execc.c\n")

    print("Привет:) Мы команда Swift!")
    print("Это наш первый проект.")
    print("Подпишитесь на наш Telegram-канал!")

    print(f"\n{ORANGE}Bug reports: @Leijan203{RESET}")

    print("\n================================")

    input("\nPress Enter to return...")


# ==============================
# START
# ==============================

if __name__ == "__main__":
    check_sudo()
    main_menu()
