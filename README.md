# Cross-Platform Logger for C and C++

A lightweight and modular logging library designed to work seamlessly across both microcontroller (MCU) environments and Linux systems.  
The library provides a pure C API as well as an optional C++ object-oriented interface.

---

## Key Features

### Cross-Platform Support
- Works on embedded MCUs (STM32, AVR, ESP32, etc.) and Linux.
- Automatic platform detection or manual override through configuration macros.

### Flexible API: C and C++
- C implementation for portability and lightweight builds.
- Optional C++ wrapper with object-oriented usage patterns.

### Configurable Log Levels
- Supported levels: `DEBUG`, `INFO`, `WARN`, `ERROR`.
- Log level can be configured at compile-time or run-time.

### Lightweight and Modular
- Small footprint suitable for constrained MCU environments.
- Modular design allows enabling or disabling platform-specific components.

### CMake-Based Build System
- Clean and structured CMake configuration.
- Simple integration into existing C or C++ projects.
- Supports building, testing, and example usage on Linux.

### Testable Architecture
- Platform abstraction helps unit testing on Linux.
- Consistent log formatting improves reproducibility in tests.
