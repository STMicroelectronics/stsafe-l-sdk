# STSAFE-L Software Development Kit

## Table of Contents

- [Hardware and Software Prerequisites](#hardware-and-software-prerequisites)
- [Getting Started](#getting-started)
  - [1. Hardware Setup](#1-hardware-setup)
  - [2. Clone the Repository](#2-clone-the-repository)
  - [3. Install STM32 Cryptographic Library (CMOX)](#3-install-stm32-cryptographic-library-cmox)
  - [4. Open and Build a Project with STM32CubeIDE](#4-open-and-build-a-project-with-stm32cubeide)
  - [5. Observe the Output](#5-observe-the-output)
  - [6. Selecting the Right Example](#6-selecting-the-right-example)
- [Repository Structure](#repository-structure)

---

The **STSAFE-L Software Development Kit** offers developers a collection of application examples demonstrating the use of the STSAFE-L secure element from STMicroelectronics.
This SDK serves as software reference implementations for integrating [STSELib](https://github.com/STMicroelectronics/STSELib) and STSAFE-L on host microcontroller platforms.

```
+------------------------+---------------------------------------------------------------------+
|                        |                                                                     |
|                        |   +-------------------------------------------------------------+   |
|       APPLICATION      |   |                 Example Projects / Tools                    |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
|========================|=====================================================================|
|                        |   +-------------------------------------------------------------+   |
|                        |   |                             |                               |   |
|                        |   |                             |                               |   |
|       MIDDLEWARE       |   |            STSELib          |       STM32Cryptolib          |   |
|                        |   |                             |           (CMOX)              |   |
|                        |   |                             |                               |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
|========================|=====================================================================|
|                        |                                                                     |
|                        |   +-------------------------------------------------------------+   |
|        PLATFORM        |   |                       NUCLEO-L452RE                         |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
+------------------------+---------------------------------------------------------------------+

```

## Hardware and Software Prerequisites

The following hardware (HW) and software (SW) prerequisites are required to work with this package:

- **Hardware prerequisites**
  - [NUCLEO-L452RE](https://www.st.com/en/evaluation-tools/nucleo-l452re.html) STM32 development board  
  - [X-NUCLEO-ESE02A1](https://www.st.com/en/evaluation-tools/x-nucleo-ese02a1.html) Nucleo expansion board

- **Software prerequisites**
  - One of the following compatible toolchains/IDEs:
    - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)  
    - [Keil uVision 5.37](https://www.st.com/en/partner-products-and-services/arm-keil-mdk.html)  
    - [IAR EWARM 9.40.1](https://www.st.com/en/partner-products-and-services/iar-embedded-workbench-for-arm.html)

  - [X-CUBE-CRYPTOLIB 4.5.0](https://www.st.com/en/embedded-software/x-cube-cryptolib.html)


## Getting Started

### 1. Hardware Setup

**Step 1 — Stack the expansion board**

Plug the [X-NUCLEO-ESE02A1](https://www.st.com/en/evaluation-tools/x-nucleo-ese02a1.html) expansion board onto the [NUCLEO-L452RE](https://www.st.com/en/evaluation-tools/nucleo-l452re.html) Arduino connectors (CN5, CN6, CN8, CN9).

> **Note:** Ensure both boards are fully seated and the power pins are correctly aligned before applying power.

**Step 2 — Connect to PC**

Connect the NUCLEO-L452RE to your PC via the USB Mini-B ST-LINK connector (CN1). The board is powered through USB; the embedded ST-LINK provides both programming/debugging and a virtual COM port for UART output.

**Step 3 — Verify enumeration**

Once connected, the PC should enumerate:
- A **ST-LINK** debug interface (used by the IDE for flashing and debugging)
- A **Virtual COM Port** (used to display example output — configure your terminal at **115200 baud, 8N1**)

---

### 2. Clone the Repository

This SDK uses git submodules for STSELib, stse-host-platform and stse-crypto-platform. Clone with `--recurse-submodules` to pull all dependencies in one step:

```bash
git clone --recurse-submodules https://github.com/STMicroelectronics/stsafe-l-sdk.git
```

If the repository was already cloned without submodules, initialise them:

```bash
git submodule update --init --recursive
```

---

### 3. Install STM32 Cryptographic Library (CMOX)

The STM32 Cryptographic library (**CMOX**) is distributed separately through [X-CUBE-CRYPTOLIB](https://www.st.com/en/embedded-software/x-cube-cryptolib.html) and is **not included** in this repository. It must be installed before building.

**Step 1 — Download**

Download **X-CUBE-CRYPTOLIB v4.5.0** from:
[https://www.st.com/en/embedded-software/x-cube-cryptolib.html](https://www.st.com/en/embedded-software/x-cube-cryptolib.html)

**Step 2 — Extract and copy**

Inside the archive, locate the middleware folder and copy its **contents** into `Middleware/STM32_Cryptographic/`:

```
en.x-cube-cryptolib-v4-5-0.zip
└── STM32CubeExpansion_Crypto_V4.5.0
    └── Middlewares\ST\STM32_Cryptographic\   ← copy contents here
```

After installation the directory must contain:

```
Middleware/STM32_Cryptographic/
├── CMOX.chm
├── LICENSE.txt
├── ReadMe.txt
├── Release_Notes.html
├── _htmresc/
├── include/
├── interface/
│   └── cmox_low_level_template.c
├── legacy_v3/
└── lib/
    ├── libSTM32Cryptographic_CM0_CM0PLUS.a
    ├── libSTM32Cryptographic_CM3.a
    ├── libSTM32Cryptographic_CM4.a       ← used by NUCLEO-L452RE (Cortex-M4)
    ├── libSTM32Cryptographic_CM7.a
    └── libSTM32Cryptographic_CM33.a
```

**Step 3 — Apply the CRC patch**

Open `Middleware/STM32_Cryptographic/interface/cmox_low_level_template.c` and comment out the two CRC lines in `cmox_ll_init()` — CRC initialisation is already handled at the project system level:

```c
cmox_init_retval_t cmox_ll_init(void *pArg)
{
  (void)pArg;
  /* CRC clock management is handled at system.c level */
//  __HAL_RCC_CRC_RELEASE_RESET();
//  __HAL_RCC_CRC_CLK_ENABLE();
  return CMOX_INIT_SUCCESS;
}
```

> See [`Middleware/STM32_Cryptographic/ReadMe.md`](Middleware/STM32_Cryptographic/ReadMe.md) for full details.

---

### 4. Open and Build a Project with STM32CubeIDE

**Step 1 — Launch STM32CubeIDE** and select or create a workspace.

**Step 2 — Import the project**

`File` → `Import` → `General` → `Existing Projects into Workspace`

Browse to `Applications/Projects/` and select the example you want to run (e.g. `SPL01_Echo_loop`). Make sure **Copy projects into workspace** is **not** checked.

**Step 3 — Build**

Right-click the project in the *Project Explorer* and select **Build Project** (or press `Ctrl+B`).  
The build output will appear in the *Console* view. A successful build produces an ELF binary under `STM32CubeIDE/Debug/`.

> **Tip:** If you see missing include errors, verify the submodules were cloned correctly (step 2).

**Step 4 — Flash and debug**

Click **Run** → **Debug** (or press `F11`) to flash the binary and start a debug session.  
To run without stopping at `main()`, click **Resume** (`F8`) immediately after the debugger connects.

---

### 5. Observe the Output

Open a serial terminal (e.g. [PuTTY](https://www.putty.org/), [Tera Term](https://teratermproject.github.io/) or the STM32CubeIDE built-in terminal) on the Virtual COM Port:

| Parameter | Value    |
|-----------|----------|
| Baud rate | 115200   |
| Data bits | 8        |
| Parity    | None     |
| Stop bits | 1        |
| Flow ctrl | None     |

Press any key when prompted (`Press key to continue`) to start the example sequence. Results are printed step by step with colour-coded **SUCCESS** / **ERROR** status.

---

### 6. Selecting the Right Example

| Project | Bus | Description |
|---------|-----|-------------|
| `SPL01_Device_authentication` | I2C | Authenticate the STSAFE-L010 using the SPL01 CA certificate |
| `SPL01_Device_authentication_multi_steps` | I2C | Step-by-step authentication — certificate parsing, challenge/response |
| `SPL01_Echo_loop` | I2C | Send and verify echoed messages — basic connectivity check |
| `SPL01_Secure_data_storage` | I2C | Read/write a counter data zone |
| `SPL02_Device_authentication` | ST1WIRE | Authenticate the STSAFE-L010 using the SPL02 CA certificate |
| `SPL02_Device_authentication_multi_steps` | ST1WIRE | Step-by-step authentication — certificate parsing, challenge/response |
| `SPL02_Echo_loop` | ST1WIRE | Send and verify echoed messages — basic connectivity check |
| `SPL02_Secure_data_storage` | ST1WIRE | Read/write a counter data zone |

> **SPL01** examples communicate over **I2C** (address `0x0C`, 100 kHz).  
> **SPL02** examples communicate over **ST1WIRE** (single-wire, address `0x00`).  
> Select the series matching your target device.

---

## Repository Structure

```
stsafe-l-sdk/
├── Applications/
│   ├── Apps_utils/                                    # Shared application utilities (terminal, buffers, delays)
│   │   ├── Apps_utils.c
│   │   └── Apps_utils.h
│   └── Projects/
│       ├── 00_Project_template/                       # Blank project template (STM32CubeIDE, Keil, IAR)
│       ├── SPL01_Device_authentication/               # SPL01 – Device authentication (I2C)
│       ├── SPL01_Device_authentication_multi_steps/   # SPL01 – Multi-step device authentication (I2C)
│       ├── SPL01_Echo_loop/                           # SPL01 – Echo loop (I2C)
│       ├── SPL01_Secure_data_storage/                 # SPL01 – Secure data storage zone access (I2C)
│       ├── SPL02_Device_authentication/               # SPL02 – Device authentication (ST1WIRE)
│       ├── SPL02_Device_authentication_multi_steps/   # SPL02 – Multi-step device authentication (ST1WIRE)
│       ├── SPL02_Echo_loop/                           # SPL02 – Echo loop (ST1WIRE)
│       └── SPL02_Secure_data_storage/                 # SPL02 – Secure data storage zone access (ST1WIRE)
├── Middleware/
│   ├── STM32_Cryptographic/                           # STM32 Cryptographic library (CMOX)
│   │   ├── include/                                   # Public API headers (cipher, hash, ECC, MAC, DRBG…)
│   │   ├── interface/                                 # Low-level platform interface template
│   │   ├── legacy_v3/                                 # Legacy v3 compatibility API
│   │   └── lib/                                       # Pre-compiled static libraries
│   └── STSELib/                                       # STSAFE-L Library — git submodule
│       ├── api/                                       # High-level device API (AES, ECC, data storage…)
│       ├── certificate/                               # X.509 certificate parsing and verification
│       ├── core/                                      # Core device and frame management
│       ├── doc/                                       # API documentation
│       └── services/                                  # Device-specific service implementations
├── stse-crypto-platform/                              # Crypto platform adaptation layer — git submodule
│   └── STSELib/                                       # CMOX-backed implementations of STSELib crypto hooks
└── stse-host-platform/                                # Host platform adaptation layer — git submodule
    ├── Core/
    │   └── CMSIS/                                     # CMSIS device support (STM32L4xx)
    ├── Drivers/                                       # Low-level hardware drivers
    │   ├── crc16/
    │   ├── delay_ms/
    │   ├── delay_us/
    │   ├── i2c/
    │   ├── rng/
    │   ├── st1wire/
    │   └── uart/
    └── STSELib/                                       # HAL implementations of STSELib platform hooks
```



