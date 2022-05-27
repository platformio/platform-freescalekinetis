# Freescale Kinetis: development platform for [PlatformIO](https://platformio.org)

[![Build Status](https://github.com/platformio/platform-freescalekinetis/workflows/Examples/badge.svg)](https://github.com/platformio/platform-freescalekinetis/actions)

Freescale Kinetis Microcontrollers is family of multiple hardware- and software-compatible ARM Cortex-M0+, Cortex-M4 and Cortex-M7-based MCU series. Kinetis MCUs offer exceptional low-power performance, scalability and feature integration.

* [Home](https://registry.platformio.org/platforms/platformio/freescalekinetis) (home page in the PlatformIO Registry)
* [Documentation](https://docs.platformio.org/page/platforms/freescalekinetis.html) (advanced usage, packages, boards, frameworks, etc.)

# Usage

1. [Install PlatformIO](https://platformio.org)
2. Create PlatformIO project and configure a platform option in [platformio.ini](https://docs.platformio.org/page/projectconf.html) file:

## Stable version

```ini
[env:stable]
platform = freescalekinetis
board = ...
...
```

## Development version

```ini
[env:development]
platform = https://github.com/platformio/platform-freescalekinetis.git
board = ...
...
```

# Configuration

Please navigate to [documentation](https://docs.platformio.org/page/platforms/freescalekinetis.html).
