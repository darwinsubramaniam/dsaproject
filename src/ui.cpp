#include "ui.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <cstdio>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "date.h"
#include "event.h"

#ifdef _WIN32
#include <io.h>
#define STDIN_IS_TTY (_isatty(_fileno(stdin)) != 0)
#else
#include <unistd.h>
#define STDIN_IS_TTY (isatty(fileno(stdin)) != 0)
#endif

namespace {

std::string pad(const std::string& text, std::size_t width) {
    if (text.size() >= width) {
        return text.substr(0, width);
    }
    return text + std::string(width - text.size(), ' ');
}

std::string rowText(const Event& e, const Date& today) {
    return pad(e.title, 22) + pad(e.subject, 14) +
           pad(e.dueDate.toString(), 13) + e.statusLabel(today);
}

std::string eventHeader() {
    return std::string("  ") + pad("Title", 22) + pad("Subject", 14) +
           pad("Due Date", 13) + "Status";
}

// Non-interactive fallback (pipes / no TTY): number the rows and read a choice.
int selectByNumber(const std::string& heading,
                   const std::string& subHeader,
                   const std::vector<std::string>& entries) {
    std::cout << "\n" << heading << "\n";
    if (!subHeader.empty()) {
        std::cout << "    " << subHeader << "\n";
    }

    for (std::size_t i = 0; i < entries.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << entries[i] << "\n";
    }

    std::cout << "Enter number (0 to cancel): ";

    int n = 0;
    if (!(std::cin >> n)) {
        std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (n < 1 || n > static_cast<int>(entries.size())) {
        return -1;
    }
    return n - 1;
}

// Arrow-key menu shared by ui::menu and ui::selectEvent. `subHeader` is an
// optional column-header line (empty to omit). Returns index or -1.
int runMenu(const std::string& heading,
            const std::string& subHeader,
            const std::vector<std::string>& entries) {
    if (entries.empty()) {
        return -1;
    }

    if (!STDIN_IS_TTY) {
        return selectByNumber(heading, subHeader, entries);
    }

    // Copy so ftxui::Menu can hold a stable pointer.
    std::vector<std::string> items = entries;

    using namespace ftxui;

    int selected = 0;
    bool confirmed = false;

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    Component theMenu = Menu(&items, &selected);

    Component withEvents = CatchEvent(theMenu, [&](ftxui::Event ev) {
        if (ev == ftxui::Event::Return) {
            confirmed = true;
            screen.Exit();
            return true;
        }
        if (ev == ftxui::Event::Escape || ev == ftxui::Event::Character('q')) {
            screen.Exit();
            return true;
        }
        return false;
    });

    Component renderer = Renderer(withEvents, [&] {
        Elements rows;
        rows.push_back(text(heading) | bold);
        if (!subHeader.empty()) {
            rows.push_back(text(subHeader) | dim);
        }
        rows.push_back(separator());
        rows.push_back(theMenu->Render() | frame);
        rows.push_back(separator());
        rows.push_back(text("Up/Down: move   Enter: select   q/Esc: cancel") | dim);
        return vbox(std::move(rows)) | border;
    });

    screen.Loop(renderer);
    return confirmed ? selected : -1;
}

}  // namespace

bool ui::interactive() {
    return STDIN_IS_TTY;
}

void ui::clearScreen() {
    if (!STDIN_IS_TTY) {
        return;
    }
    // Clear the screen and scrollback, then move the cursor to the top-left.
    std::cout << "\033[2J\033[3J\033[H" << std::flush;
}

int ui::menu(const std::string& heading, const std::vector<std::string>& options) {
    return runMenu(heading, "", options);
}

int ui::selectEvent(const std::string& heading,
                    const std::vector<Event>& events,
                    const Date& today) {
    std::vector<std::string> entries;
    entries.reserve(events.size());
    for (const Event& e : events) {
        entries.push_back(rowText(e, today));
    }
    return runMenu(heading, eventHeader(), entries);
}
