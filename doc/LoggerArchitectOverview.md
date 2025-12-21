# Logger System Architecture – Big Picture Overview

The logger system is designed using layered architecture to ensure clear
separation of concerns, portability across MCU and Linux environments, and
long-term maintainability. Each layer has a specific responsibility and strict
boundaries that prevent leakage of platform details into high-level logic.

This document describes the overall architecture as represented in the big-picture
diagram.

---

## 1. Application Layer

The Application Layer represents firmware modules, services, tasks, or any
business logic that requires logging.  
It **does not implement logging logic** and does not interact directly with sinks,
platform IO, or policies.

The application only calls the **Logger API**, making the system highly portable.

**Responsibilities:**
- Emit log requests with severity, message, and module information.
- Remain agnostic to logger internals and output destinations.

**Non-Responsibilities:**
- Managing policy rules  
- Formatting messages  
- Routing logs  
- Handling IO  

---

## 2. Logger Frontend (API Layer)

The API Layer is the **only interface exposed to the application**.  
It translates application log requests into a structure the Core can understand.

**Responsibilities:**
- Provide macros and lightweight functions (`LOG_INFO`, `LOG_ERROR`, etc.)
- Perform minimal preprocessing (severity filtering, fast path elimination)
- Forward log events into the Core

**Non-Responsibilities:**
- Evaluating policies  
- Dispatch logic  
- Platform IO  
- Message formatting  

The API layer acts as a ** façade ** that completely hides internal details.

---

## 3. Policy Layer (Configuration Rules)

The Policy Layer defines the rules governing how log events are processed.  
Policies are configured through build settings or runtime configuration (Linux).

**Responsibilities:**
- Establish minimum severity levels
- Control rate limiting or event throttling
- Enable/disable logging globally or per module
- Prevent recursion or unsafe logging states

**Non-Responsibilities:**
- Creating log events
- Performing IO
- Routing logs

Policies are **evaluated by the Core**, but not implemented inside it.

---

## 4. Core Logging Engine

The Core is the **heart of the system**, containing the logic needed to transform
a log event into a dispatch decision.

**Responsibilities:**
- Construct, validate, and finalize log events
- Apply all policy rules
- Determine dispatch behavior (drop, fan-out, single target)
- Handle failure cases safely
- Maintain internal state and metrics

**Non-Responsibilities:**
- Performing IO  
- Knowing about platform specifics  
- Knowing about sinks or formatting  
- Exposing interfaces directly to the application  

The Core is intentionally platform-agnostic, deterministic, and MCU-safe.

---

## 5. Transport / Sink Layer

The Transport Layer defines **where** accepted log events are delivered.  
Each sink corresponds to a specific output mechanism.

Examples:
- UART sink
- File sink
- Console sink
- Syslog sink
- Ring buffer sink

**Responsibilities:**
- Receive finalized events from the Core
- Deliver events to actual output channels
- Optionally apply formatting (if configured)

**Non-Responsibilities:**
- Policy evaluation  
- Event creation or validation  
- Platform abstraction details  

Sinks **implement output**, but do not dictate routing logic.

---

## 6. Platform Abstraction Layer (PAL)

The PAL isolates platform-specific operations so the core and API remain pure.

**Responsibilities:**
- Provide time sources (ticks on MCU, monotonic clock on Linux)
- Provide synchronization primitives (if enabled)
- Expose low-level IO mechanisms required by sinks
- Expose OS/HAL integration points

**Non-Responsibilities:**
- Logging logic  
- Event processing  
- Policy rules  
- Dispatch planning  

PAL ensures portability by allowing the same logger core to run on:
- Bare-metal MCU  
- RTOS (FreeRTOS, Zephyr, …)  
- Linux  

---

## 7. End-to-End Data Flow (Conceptual)

```
Application
    ↓
Logger API
    ↓
Policy Layer
    ↓
Core Logging Engine
    ↓
Transport layer
    ↓
Platform Abstraction Layer
    ↓
Actual Output (file, console, UART, syslog)
```

## 8. Folder Structure

```
src/
└── Logger/
    ├── ApiInterface/
    ├── CoreProcessing/
    ├── Policy/
    ├── OutputSinks/
    ├── Platforms/
    └── Examples/
```