#ifndef STORE_H
#define STORE_H

class Dashboard;

// CSV-backed persistence for the dashboard's events (events.csv).
namespace store {

// Load events from events.csv into the dashboard. No-op (with a message) if
// the file is absent.
void load(Dashboard& dash);

// Write all of the dashboard's events to events.csv.
void save(const Dashboard& dash);

}  // namespace store

#endif  // STORE_H
