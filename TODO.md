# Project Roadmap and Feature Checklist

This file tracks planned enhancements and features for the Logger library. Items can be checked off as they are implemented.

---

## 1. Rolling / Rotating File Backend -> low priority

- [ ] Add file size threshold configuration (e.g. `max_file_size_mb`).
- [ ] Implement size-based rotation logic in `FileBackend` (archive to `app.log.1`, `app.log.2`, etc.).
- [ ] Implement backup file count retention based on `max_backups`.
- [ ] Add optional date/time-based rotation (e.g. `app-YYYY-MM-DD.log`).
- [ ] Add unit tests for file rotation and backup deletion.

---

## 2. Source Location Support (`__FILE__`, `__LINE__`, `__func__`) -> high priority

- [x] Create `SourceLocation` metadata structure or integrate `std::source_location` / compiler macros.
- [x] Update `Logger::printMessage()` to accept source location parameters.
- [x] Update `LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, and `LOG_ERROR` macros to capture `__FILE__`, `__LINE__`, and `__func__`.
- [x] Update `LogFormatter` to format `m_file` and `m_line` fields when enabled in YAML configuration.
- [x] Add unit tests verifying file name and line number in formatted output.

---

## 3. Asynchronous Logging Backend (`AsyncLoggerWorker`) -> medium priority

- [ ] Implement thread-safe bounded lock-free or ring-buffer queue for log messages.
- [ ] Implement background worker thread for asynchronous backend flushing.
- [ ] Add configurable queue overflow policies (block, drop oldest, or fallback to sync).
- [ ] Provide graceful shutdown and flush mechanism on application exit or crash handlers.
- [ ] Benchmark async throughput and latency against synchronous mode.

---

## 4. ANSI Colorized Console Output -> low priority

- [ ] Implement terminal color codes (ANSI escape sequences) for log levels:
  - `VERBOSE` / `DEBUG`: Cyan / Dim
  - `INFO`: Green
  - `WARNING`: Yellow
  - `ERROR`: Red / Bold
- [ ] Add TTY detection (`isatty`) to disable ANSI colors automatically when redirected to file or pipe.
- [ ] Add configuration option in YAML to enable/disable colored output.
- [ ] Add unit tests verifying color formatting behavior.

---

## 5. Network / UDP / Syslog Backend (`NetworkBackend`) -> low priority

- [ ] Implement `NetworkBackend` inheriting from `ILogBackend`.
- [ ] Add non-blocking UDP socket transport for remote syslog / log aggregators.
- [ ] Add support for UNIX domain sockets (`/dev/log`) for native Linux syslog integration.
- [ ] Add configuration parser support for network target (host, port, protocol) in `LogConfig.yaml`.
- [ ] Add unit and integration tests with local UDP socket listener mock.

---

## 6. Per-Module Dynamic Level Filtering & Control -> medium priority

- [ ] Add support for per-module minimum log levels (e.g. `Logger::setModuleLevel("NETWORK", LogLevel::WARNING)`).
- [ ] Add YAML configuration section for per-module level overrides:
  ```yaml
  logging:
    modules:
      Network: WARNING
      Database: DEBUG
      Core: INFO
  ```
- [ ] Implement fast hash-map lookup during fast-path filtering in `printMessage()`.
- [ ] Add unit tests verifying independent module-level filtering.

---

## 7. Shared Memory / MMap Crash Recovery Buffer (`SharedMemoryBackend`) -> high priority

- [x] Implement POSIX shared memory ring-buffer (`shm_open` / `mmap` at `/dev/shm/logger_buffer.shm`) or memory-mapped cache.
- [x] Continuously record console log stream into kernel-backed shared memory with zero disk I/O overhead.
- [x] Ensure logs persist and survive sudden process crashes (`SIGSEGV`, `SIGABRT`, `std::terminate`) outside the process address space.
- [x] Implement standalone reader tool / API (`Logger::recoverCrashLogs()`) to inspect and extract historical logs after a crash.
- [x] Add automatic crash recovery dump on application restart to check and retrieve previous crash session logs.
- [x] Add unit tests verifying log recovery from shared memory after simulated process abortion.

---

## 8. Rate Limiting, Throttling & Assertion Checks (`LOG_ASSERT` / `LOG_CHECK`) -> medium priority

- [ ] Implement `LOG_ASSERT(condition, message, ...)` macro:
  - Evaluates boolean condition expression.
  - On failure, logs `ERROR`/`FATAL` with expression string, file, line, and custom formatted message.
  - Configurable failure policy (abort via `std::abort`, throw exception, or break into debugger).
- [ ] Implement `LOG_CHECK(condition, message, ...)` macro for non-fatal runtime verification (logs warning/error without terminating).
- [ ] Implement `LOG_EVERY_N(N, LEVEL, ...)` macro to log once every $N$ invocations.
- [ ] Implement `LOG_FIRST_N(N, LEVEL, ...)` macro to log only the first $N$ times.
- [ ] Implement `LOG_THROTTLE(SECONDS, LEVEL, ...)` macro to limit output frequency per call site.
- [ ] Implement consecutive duplicate log suppression with `Last message repeated N times` notification.
- [ ] Add unit tests for assertion triggers and rate limiting logic.

---

## 9. Crash & Signal Handler with Stacktrace Dump -> high priority

- [x] Intercept fatal signals (`SIGSEGV`, `SIGABRT`, `SIGBUS`, `SIGFPE`, `SIGILL`).
- [x] Capture and format stack trace backtraces (using `backtrace()` / `backtrace_symbols()` or C++23 `<stacktrace>`).
- [x] Synchronously flush all pending backend buffers prior to process termination.
- [x] Provide optional registration API (`Logger::installCrashHandler()`).
- [x] Add test cases verifying signal capture and stacktrace generation.

---

## 10. Custom Pattern String Formatter (`PatternFormatter`) -> medium priority

- [ ] Implement flexible pattern-based formatter engine (e.g. `pattern: "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%n] %v"`).
- [ ] Support format flags:
  - `%Y-%m-%d %H:%M:%S.%e`: Timestamps with sub-second precision (ms/us)
  - `%l`: Short level name (`I`, `W`, `E`) or full name (`INFO`, `WARNING`, `ERROR`)
  - `%n`: Logger/module name
  - `%t`: Thread ID
  - `%s` / `%#`: Source file and line number
  - `%v`: Actual log message
- [ ] Add YAML parser support for custom pattern string specification.
- [ ] Add unit tests covering pattern token substitution and custom formatting.

---

## 11. Configurable Field Ordering / Sequence Formatter -> high priority

- [x] Store ordered sequence of fields in `LogFormat` (e.g. `std::vector<FieldType> m_fieldOrder`) instead of fixed boolean flags.
- [x] Parse `fields:` list preserving exact YAML declaration order:
  - e.g., `[timestamp, module, level, message]` vs `[timestamp, level, module, message]`.
- [x] Update `LogFormatter::format()` to iterate through `m_fieldOrder` dynamically to construct parts in the exact requested sequence.
- [x] Support arbitrary ordering for any combination of `timestamp`, `level`, `module`, `thread_id`, `file`, `line`, and `message`.
- [x] Add unit tests verifying different field order permutations.
