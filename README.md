# C++ Modular & Configurable Logging Library

A thread-safe, high-performance, and modular C++ logging library featuring `{fmt}` formatting, multi-backend routing, runtime log level filtering, and YAML-based configuration.

---

## Key Features

- **Modern C++ & `{fmt}` Formatting**: Type-safe, fast string formatting with positional argument support.
- **Multiple Output Backends**:
  - `ConsoleBackend`: Outputs logs directly to `stdout`.
  - `FileBackend`: Logs to files with customizable output paths.
  - Extensible `ILogBackend` interface for adding custom network, syslog, or hardware backends.
- **YAML Configuration**:
  - Configure backends (`console`, `file`, enabled status, per-backend minimum log levels, file paths).
  - Configure formatting fields (`timestamp`, `level`, `module`, `message`) and field separators.
- **Runtime Log Level Filtering**:
  - Supported levels: `NONE`, `VERBOSE`, `DEBUG`, `INFO`, `WARNING`, `ERROR`.
  - Hierarchical filtering at the logger level and individual backend level.
- **Global Provider & Dependency Injection**:
  - Easy-to-use logging macros (`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR`) backed by `LogProvider`.
  - Full support for dependency injection by instantiating standalone `Logger` objects.
- **Thread-Safe**: Safe for concurrent logging across multi-threaded applications.

---

## Project Architecture

```
logger/
├── include/
│   ├── ILogBackend.hpp              # Abstract interface for log output sinks
│   ├── ConsoleBackend.hpp          # Console output backend
│   ├── FileBackend.hpp             # File output backend
│   ├── LogLevel.hpp                # LogLevel and BackendType enums (Better Enums)
│   ├── LogFormatterConfigParser.hpp # YAML configuration parser
│   ├── LogFormatter.hpp            # Formats log records with timestamps and metadata
│   ├── Logger.hpp                  # Core Logger engine dispatching to backends
│   ├── LogProvider.hpp             # Provider/Service registry for default logger
│   └── Log.hpp                     # Convenience macros (LOG_INFO, LOG_DEBUG, etc.)
├── src/                            # Implementation files
├── example/
│   ├── config/LogConfig.yaml       # Example YAML configuration
│   └── main.cpp                    # Usage demonstrations and benchmarks
├── test/                           # GoogleTest and GoogleMock unit tests
└── scripts/                        # Docker and native build helper scripts
```

---

## Configuration Format

You can configure the logger using a YAML configuration file (e.g. `LogConfig.yaml`):

```yaml
logging:
  format:
    fields:
      - timestamp
      - level
      - module
      - message
    separator: " | "
  backends:
    console:
      enabled: true
      level: INFO
    file:
      enabled: true
      level: DEBUG
      path: "../app.log"
      max_backups: 1
```

---

## Quick Start & Usage

### 1. Using YAML Configuration with Global Macros

```cpp
#include "Logger.hpp"
#include "LogProvider.hpp"
#include "Log.hpp"

int main()
{
    // Initialize logger with YAML configuration
    auto logger = std::make_shared<Helper::Logger::Logger>("example/config/LogConfig.yaml");
    Helper::Logger::setDefaultLogger(logger);

    // Log messages using macros
    LOG_INFO("Application started successfully on port {}", 8080);
    LOG_DEBUG("Debugging calculation: {} + {} = {}", 2, 2, 4);
    LOG_WARNING("Resource usage high: {}%", 88.5);
    LOG_ERROR("Failed to connect to database: {}", "timeout");

    return 0;
}
```

### 2. Programmatic Configuration

```cpp
#include "Logger.hpp"
#include "ConsoleBackend.hpp"
#include "FileBackend.hpp"
#include "LogProvider.hpp"
#include "Log.hpp"

int main()
{
    // Create empty logger without YAML file
    auto logger = std::make_shared<Helper::Logger::Logger>("");
    
    // Add backends dynamically
    logger->addBackend(std::make_shared<Helper::Logger::ConsoleBackend>());
    logger->addBackend(std::make_shared<Helper::Logger::FileBackend>("run.log"));
    
    // Set minimum log level threshold
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    
    Helper::Logger::setDefaultLogger(logger);

    LOG_INFO("Ready to process requests");
    return 0;
}
```

---

## Build & Test Instructions

### Using Docker Development Environment (Recommended)

The repository provides a Docker container with all preconfigured dependencies (Clang, CMake, GoogleTest, `{fmt}`, etc.).

```bash
# Build, run linter, compile, and run examples inside Docker
./scripts/run_dev.sh ./scripts/build_native.sh all

# Or run individual steps:
./scripts/run_dev.sh ./scripts/build_native.sh configure
./scripts/run_dev.sh ./scripts/build_native.sh lint
./scripts/run_dev.sh ./scripts/build_native.sh build
./scripts/run_dev.sh ./scripts/build_native.sh run
```

### Running Unit Tests

```bash
./scripts/run_dev.sh ./build/test/test_logger
```

### Native Build (Host Machine)

If you have CMake (>= 3.16) and a C++17/20 compiler installed:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
./build/test/test_logger
./build/example/main_logger
```
