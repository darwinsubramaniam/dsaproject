# E-Learning Dashboard

MECS0023 Data Structure & Algorithm mini project — a console dashboard that
tracks e-learning events (quizzes, assignments) with deadlines, reminders, and
overdue alerts, persisted to `events.csv`.

The data structures are hand-rolled on purpose (the point of the assignment):

- **Singly-linked list**, kept sorted by due date — the event store (`Dashboard`)
- **Queue** — pending reminders (`ReminderQueue`)
- **Stack** — overdue alerts (`OverdueStack`)
- **Selection sort** — sort events by date / subject / status

CSV reading/writing uses the vendored, header-only
[rapidcsv](https://github.com/d99kris/rapidcsv) library.

## Layout

```
.
├── include/            # our headers (interfaces)
│   ├── constants.h
│   ├── date.h
│   ├── event.h
│   ├── node.h
│   ├── reminder_queue.h
│   ├── overdue_stack.h
│   └── dashboard.h
├── src/                # implementations
│   ├── date.cpp
│   ├── event.cpp
│   ├── reminder_queue.cpp
│   ├── overdue_stack.cpp
│   ├── dashboard.cpp
│   └── main.cpp        # file loading + menu loop
├── third_party/        # vendored dependencies
│   └── rapidcsv.h      # single-header CSV library (BSD-3-Clause)
├── CMakeLists.txt      # cross-platform build (Windows / macOS / Linux)
└── Makefile            # convenience build for Unix-like shells
```

## Build & run

### CMake (recommended — works on Windows, macOS, Linux)

```sh
cmake -S . -B build      # configure
cmake --build build      # compile -> build/dashboard (build/dashboard.exe on Windows)
./build/dashboard        # run
```

On Windows this generates the right project for your toolchain (Visual Studio,
MinGW, Ninja, …); no Unix shell required.

### Make (Unix-like shells: Linux, macOS, MSYS2/Git Bash on Windows)

```sh
make        # compile and link -> ./dashboard
make run    # build, then run
make clean  # remove obj/ and the binary
```

Requires a C++17 compiler. On first run, if `events.csv` is absent the program
says so; it creates the file when you add or save events.

## Data format

`events.csv` has a header row followed by one event per line:

```
title,subject,type,day,month,year,completed
Quiz 1,Math,Quiz,1,6,2026,0
"Report, final",Science,Assignment,10,6,2026,1
```

- `completed` is `1` (done) or `0` (pending); the column is optional on load
  and defaults to `0`.
- Fields containing commas or quotes are quoted automatically (standard CSV).
- Completing an event sets its status to `Completed` rather than deleting it,
  so the record is kept for traceability.
