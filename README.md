# E-Learning Dashboard

MECS0023 Data Structure & Algorithm mini project — a console dashboard that
tracks e-learning events (quizzes, assignments) with deadlines, reminders, and
overdue alerts, persisted to `events.csv`.

The data structures are hand-rolled on purpose (the point of the assignment):

- **Singly-linked list**, kept sorted by due date — the event store (`Dashboard`)
- **Queue** — pending reminders (`ReminderQueue`)
- **Stack** — overdue alerts (`OverdueStack`)
- **Selection sort** — sort events by date / subject / status

Third-party libraries:

- [rapidcsv](https://github.com/d99kris/rapidcsv) — vendored single header
  (`third_party/`), used for `events.csv` reading/writing.
- [FTXUI](https://github.com/ArthurSonzogni/FTXUI) — terminal UI, fetched and
  built automatically by CMake. Powers the arrow-key event picker used by
  Amend / Delete / Mark Complete (falls back to a numbered prompt when stdin
  isn't a terminal).

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
│   ├── dashboard.h
│   ├── store.h         # events.csv persistence
│   └── ui.h            # interactive event picker (FTXUI)
├── src/                # implementations
│   ├── date.cpp
│   ├── event.cpp
│   ├── reminder_queue.cpp
│   ├── overdue_stack.cpp
│   ├── dashboard.cpp
│   ├── store.cpp
│   ├── ui.cpp
│   └── main.cpp        # menu loop
├── third_party/        # vendored dependencies
│   └── rapidcsv.h      # single-header CSV library (BSD-3-Clause)
├── CMakeLists.txt      # cross-platform build (Windows / macOS / Linux)
└── Makefile            # convenience wrapper that delegates to CMake
```

## Build & run

Requires a **C++20** compiler. The first build downloads and compiles FTXUI
(needs network access once); subsequent builds are incremental.

The **default build is unoptimized** for fast iteration. A **Release** build
adds link-time optimization, per-function/data sections, linker dead-stripping,
and symbol stripping, which drop unused FTXUI code from the final binary
(≈360 KB vs ≈2.9 MB) — at the cost of a slower (LTO) build.

### CMake (works on Windows, macOS, Linux)

```sh
cmake -S . -B build                          # fast default (fetches FTXUI once)
cmake --build build                          # -> build/dashboard (.exe on Windows)
./build/dashboard

# Optimized / small binary:
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Make (delegates to CMake)

```sh
make          # fast, unoptimized build -> build/dashboard
make release  # optimized, size-minimized build (LTO + strip)
make run      # fast build, then run
make clean    # remove build/
```

On first run, if `events.csv` is absent the program says so; it creates the
file when you add or save events.

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
