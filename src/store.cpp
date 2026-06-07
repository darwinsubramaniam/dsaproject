#include "store.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dashboard.h"
#include "date.h"
#include "event.h"
#include "rapidcsv.h"

namespace store {

namespace {
const char* const kFilePath = "events.csv";
}  // namespace

// CSV columns: title,subject,type,day,month,year,completed
// The "completed" column is optional (defaults to not completed).
void load(Dashboard& dash) {
    std::ifstream probe(kFilePath);

    if (!probe) {
        std::cout << "events.csv not found.\n";
        std::cout << "It will be created when you add or save events.\n";
        return;
    }
    probe.close();

    rapidcsv::Document doc(kFilePath, rapidcsv::LabelParams(0, -1));

    std::vector<std::string> columns = doc.GetColumnNames();
    bool hasCompleted =
        std::find(columns.begin(), columns.end(), "completed") != columns.end();

    for (std::size_t i = 0; i < doc.GetRowCount(); i++) {
        try {
            Date dueDate(doc.GetCell<int>("day", i),
                         doc.GetCell<int>("month", i),
                         doc.GetCell<int>("year", i));

            if (!dueDate.isValid()) {
                continue;  // skip rows with an impossible date (e.g. 31 Feb)
            }

            Event e{doc.GetCell<std::string>("title", i),
                    doc.GetCell<std::string>("subject", i),
                    doc.GetCell<std::string>("type", i),
                    dueDate};

            if (hasCompleted) {
                e.completed = (doc.GetCell<int>("completed", i) == 1);
            }

            dash.addEvent(e);
        } catch (const std::exception&) {
            continue;  // skip malformed rows
        }
    }

    std::cout << "Events loaded from events.csv.\n";
}

void save(const Dashboard& dash) {
    // Build an in-memory CSV document (row 0 holds the column names) and let
    // rapidcsv handle quoting/escaping on write.
    rapidcsv::Document doc(std::string(), rapidcsv::LabelParams(0, -1));
    doc.SetColumnName(0, "title");
    doc.SetColumnName(1, "subject");
    doc.SetColumnName(2, "type");
    doc.SetColumnName(3, "day");
    doc.SetColumnName(4, "month");
    doc.SetColumnName(5, "year");
    doc.SetColumnName(6, "completed");

    int row = 0;

    for (const Event& e : dash.events()) {
        const Date& d = e.dueDate;

        doc.SetCell<std::string>(0, row, e.title);
        doc.SetCell<std::string>(1, row, e.subject);
        doc.SetCell<std::string>(2, row, e.type);
        doc.SetCell<int>(3, row, d.getDay());
        doc.SetCell<int>(4, row, d.getMonth());
        doc.SetCell<int>(5, row, d.getYear());
        doc.SetCell<int>(6, row, e.completed ? 1 : 0);

        row++;
    }

    try {
        doc.Save(kFilePath);
    } catch (const std::exception&) {
        std::cout << "Error: Cannot save events.csv.\n";
    }
}

}  // namespace store
