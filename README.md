# Freescale Kinetis: development platform for [PlatformIO](http://platformio.org)
[![Build Status](https://travis-ci.org/platformio/platform-freescalekinetis.svg?branch=develop)](https://travis-ci.org/platformio/platform-freescalekinetis)
[![Build status](https://ci.appveyor.com/api/projects/status/gbf7v0d19qir2o05/branch/develop?svg=true)](https://ci.appveyor.com/project/ivankravets/platform-freescalekinetis/branch/develop)

Freescale Kinetis Microcontrollers is family of multiple hardware- and software-compatible ARM Cortex-M0+, Cortex-M4 and Cortex-M7-based MCU series. Kinetis MCUs offer exceptional low-power performance, scalability and feature integration.

* [Home](http://platformio.org/platforms/freescalekinetis) (home page in PlatformIO Platform Registry)
* [Documentation](http://docs.platformio.org/page/platforms/freescalekinetis.html) (advanced usage, packages, boards, frameworks, etc.)

# Usage

1. [Install PlatformIO](http://platformio.org)
2. Create PlatformIO project and configure a platform option in [platformio.ini](http://docs.platformio.org/page/projectconf.html) file:

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

Please navigate to [documentation](http://docs.platformio.org/page/platforms/freescalekinetis.html).
