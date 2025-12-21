# Core Processing Overview

The Core Processing component is the central logic engine of the logging system.
Its purpose is to transform incoming log events into routing decisions based on
a set of deterministic policies. The Core is strictly platform-agnostic and
performs no input/output operations.

Core Processing receives log requests from the API layer, evaluates them against
configured rules, and outputs dispatch decisions to the output sink layer.

---

## Core Processing Responsibilities

The Core is responsible for the following actions:

### 1. Event Construction and Validation
- Construct internal log event representations.
- Validate severity, timestamp, formatting state, and payload length.
- Ensure the event enters processing in a clean and consistent state.

### 2. Policy Evaluation
- Apply configured policies (severity threshold, rate limiting, reentrancy, etc.).
- Determine whether a log event is accepted or rejected.
- Guarantee deterministic behavior under all system conditions.

### 3. Dispatch Decision Making
- Decide whether to output, drop, or fan out the event to multiple sinks.
- Apply dispatch rules independent of platform or sink details.
- Produce a routing decision object for the output layer.

### 4. Failure Handling
- Handle dropped or rejected log events safely without recursion.
- Update internal counters for drops, throttles, or policy rejects.
- Ensure predictable behavior even in failure or overload scenarios.

### 5. Maintaining Core State
- Maintain lightweight metrics such as accepted/dropped counters.
- Manage shared state required by policies (rate counters, timestamps).
- Ensure no platform-specific dependencies exist in this state.

---

## Core Processing Non-Responsibilities

The Core must not perform any of the following:

- Produce console, UART, file, or network output.
- Access hardware or operating system services directly.
- Allocate platform-specific resources.
- Format log messages into text or binary representations.
- Manage user configuration or front-end API concerns.
- Interact directly with sinks or platform details.

Any violation of these rules breaks architectural integrity.

---

## Core Processing Structure

The internal directory structure is organized as:

```
src/
└── Logger/
    └── CoreProcessing/
        ├── CoreProcessing/
        ├── EventModel/
        ├── EventLifecycle/
        ├── PolicyEngine/
        ├── DispatchEngine/
        ├── FailureHandling/
        └── CoreState/
```

Each directory maps to a specific responsibility within Core Processing.

---

### EventModel/
Defines the internal representation of a log event.

Includes:
- Log event structures
- Severity and metadata types

---

### EventLifecycle/
Responsible for:
- Creating new events
- Validating event fields
- Finalizing events into immutable state

---

### PolicyEngine/
Evaluates policies that determine:
- Acceptance or rejection
- Throttling or limitation
- Recursion handling
- Module-level enabling/disabling

---

### DispatchEngine/
Responsible for:
- Computing dispatch decisions
- Fan-out or single-target logic
- Routing semantics independent of sinks

---

### FailureHandling/
Defines behavior for:
- Handling rejected events
- Managing drops and errors
- Updating drop metrics

---

### CoreState/
Stores:
- Internal metrics (accepted/dropped counters)
- State shared between core subsystems

---

## Core Data Flow

The sequence below defines how a log event moves through Core Processing:

```
Incoming Log Request
    ↓
EventLifecycle (create → validate → finalize)
    ↓
PolicyEngine (apply rules → accept/reject)
    ↓
DispatchEngine (compute routing decision)
    ↓
Output Layer (handled by OutputSinks)
```
