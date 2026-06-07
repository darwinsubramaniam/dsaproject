# E-Learning Dashboard

MECS0023 Data Structure & Algorithm mini project — a console dashboard that
tracks e-learning events (quizzes, assignments) with deadlines, reminders, and
overdue alerts, persisted to `events.txt`.

The data structures are hand-rolled on purpose (the point of the assignment):

- **Singly-linked list**, kept sorted by due date — the event store (`Dashboard`)
- **Queue** — pending reminders (`ReminderQueue`)
- **Stack** — overdue alerts (`OverdueStack`)
- **Selection sort** — sort events by date / subject / status

## Layout

```
.
├── include/        # headers (interfaces)
│   ├── constants.h
│   ├── date.h
│   ├── event.h
│   ├── node.h
│   ├── reminder_queue.h
│   ├── overdue_stack.h
│   └── dashboard.h
├── src/            # implementations
│   ├── date.cpp
│   ├── event.cpp
│   ├── reminder_queue.cpp
│   ├── overdue_stack.cpp
│   ├── dashboard.cpp
│   └── main.cpp    # file loading + menu loop
├── build/          # compiled objects (generated)
└── Makefile
```

## Build & run

```sh
make        # compile and link -> ./dashboard
make run    # build, then run
make clean  # remove build/ and the binary
```

Requires a C++17 compiler. On first run, if `events.txt` is absent the program
says so; it creates the file when you add or save events.

## Data format

`events.txt` is one event per line, semicolon-separated:

```
title;subject;type;day;month;year
```
