#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

#include "date.h"
#include "event.h"

namespace ui {

// True when stdin is an interactive terminal (so the arrow-key UI is usable).
bool interactive();

// Arrow-key menu over `options`. Returns the chosen index, or -1 on q/Esc.
// Falls back to a numbered prompt when stdin is not a terminal.
int menu(const std::string& heading, const std::vector<std::string>& options);

// Show an interactive picker over `events` (arrow keys + Enter, q/Esc to
// cancel). Returns the chosen index into `events`, or -1 if cancelled or the
// list is empty. Falls back to a numbered prompt when stdin is not a terminal.
int selectEvent(const std::string& heading,
                const std::vector<Event>& events,
                const Date& today);

}  // namespace ui

#endif  // UI_H
