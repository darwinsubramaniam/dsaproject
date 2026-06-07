// =====================================================================
// MECS0023 DATA STRUCTURE & ALGORITHM - MINI PROJECT
// E-Learning Dashboard using events.txt
// =====================================================================

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ------------------------- class Date --------------------------------
class Date {
private:
    int day;
    int month;
    int year;

public:
    Date() {
        day = 1;
        month = 1;
        year = 2026;
    }

    Date(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    }

    int getDay() {
        return day;
    }

    int getMonth() {
        return month;
    }

    int getYear() {
        return year;
    }

    long toKey() {
        return (long)year * 10000 + month * 100 + day;
    }

    long toDayNumber() {
        return (long)year * 365 + month * 30 + day;
    }

    int compareTo(Date other) {
        if (toKey() < other.toKey()) {
            return -1;
        } else if (toKey() > other.toKey()) {
            return 1;
        } else {
            return 0;
        }
    }

    void display() {
        if (day < 10) {
            cout << "0";
        }

        cout << day << "/";

        if (month < 10) {
            cout << "0";
        }

        cout << month << "/" << year;
    }
};

// ------------------------- class Event -------------------------------
class Event {
private:
    string title;
    string subject;
    string type;
    Date dueDate;

public:
    Event() {
        title = "";
        subject = "";
        type = "";
    }

    Event(string t, string s, string ty, Date d) {
        title = t;
        subject = s;
        type = ty;
        dueDate = d;
    }

    string getTitle() {
        return title;
    }

    string getSubject() {
        return subject;
    }

    string getType() {
        return type;
    }

    Date getDueDate() {
        return dueDate;
    }

    void setDueDate(Date d) {
        dueDate = d;
    }

    string getStatus(Date today) {
        long diff = dueDate.toDayNumber() - today.toDayNumber();

        if (diff < 0) {
            return "Past Deadline";
        } else if (diff == 0) {
            return "Upcoming Today";
        } else {
            return "Upcoming (" + to_string(diff) + " days)";
        }
    }

    int statusRank(Date today) {
        long diff = dueDate.toDayNumber() - today.toDayNumber();

        if (diff < 0) {
            return 0;
        } else {
            return 1;
        }
    }
};

// ------------------------- class Node --------------------------------
class Node {
public:
    Event data;
    Node* next;

    Node(Event e) {
        data = e;
        next = NULL;
    }
};

// ------------------------- Queue -------------------------------------
class ReminderQueue {
private:
    string items[100];
    int front;
    int rear;

public:
    ReminderQueue() {
        front = 0;
        rear = 0;
    }

    bool isEmpty() {
        return front == rear;
    }

    void enqueue(string title) {
        if (rear < 100) {
            items[rear] = title;
            rear++;
        }
    }

    void clear(string title) {
        int i = front;

        while (i < rear) {
            if (items[i] == title) {
                int j = i;

                while (j < rear - 1) {
                    items[j] = items[j + 1];
                    j++;
                }

                rear--;
                return;
            }

            i++;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "No reminders.\n";
            return;
        }

        int i = front;
        int no = 1;

        while (i < rear) {
            cout << no << ". Reminder: " << items[i] << "\n";
            i++;
            no++;
        }
    }
};

// ------------------------- Stack -------------------------------------
class OverdueStack {
private:
    string items[100];
    int top;

public:
    OverdueStack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool contains(string title) {
        int i = 0;

        while (i <= top) {
            if (items[i] == title) {
                return true;
            }

            i++;
        }

        return false;
    }

    void push(string title) {
        if (top < 99) {
            top++;
            items[top] = title;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "No overdue alerts.\n";
            return;
        }

        int i = top;
        int no = 1;

        while (i >= 0) {
            cout << no << ". OVERDUE: " << items[i] << "\n";
            i--;
            no++;
        }
    }
};

// ------------------------- Dashboard ---------------------------------
class Dashboard {
private:
    Node* head;
    ReminderQueue reminders;
    OverdueStack alerts;

    void printPadded(string text, int width) {
        cout << text;

        int i = text.length();

        while (i < width) {
            cout << " ";
            i++;
        }
    }

public:
    Dashboard() {
        head = NULL;
    }

    ~Dashboard() {
        Node* current = head;

        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addEvent(Event e) {
        Node* newNode = new Node(e);

        if (head == NULL || e.getDueDate().compareTo(head->data.getDueDate()) < 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;

            while (current->next != NULL &&
                   current->next->data.getDueDate().compareTo(e.getDueDate()) < 0) {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;
        }

        reminders.enqueue(e.getTitle());
    }

    bool deleteEvent(string title) {
        Node* target = NULL;

        if (head != NULL && head->data.getTitle() == title) {
            target = head;
            head = head->next;
        } else {
            Node* current = head;

            while (current != NULL &&
                   current->next != NULL &&
                   current->next->data.getTitle() != title) {
                current = current->next;
            }

            if (current != NULL && current->next != NULL) {
                target = current->next;
                current->next = target->next;
            }
        }

        if (target != NULL) {
            delete target;
            reminders.clear(title);
            return true;
        }

        return false;
    }

    void amendEvent(string title, Date newDate) {
        Node* current = head;

        while (current != NULL) {
            if (current->data.getTitle() == title) {
                Event e = current->data;
                e.setDueDate(newDate);

                deleteEvent(title);
                addEvent(e);

                cout << "Event amended.\n";
                return;
            }

            current = current->next;
        }

        cout << "Event not found.\n";
    }

    void checkDeadlines(Date today) {
        Node* current = head;

        while (current != NULL) {
            if (current->data.getStatus(today) == "Past Deadline" &&
                !alerts.contains(current->data.getTitle())) {
                alerts.push(current->data.getTitle());
            }

            current = current->next;
        }
    }

    void displayTimeline(Date today) {
        checkDeadlines(today);

        if (head == NULL) {
            cout << "No events found.\n";
            return;
        }

        cout << "No  Title                      Subject     Type        Due Date     Status\n";
        cout << "-------------------------------------------------------------------------------\n";

        Node* current = head;
        int no = 1;

        while (current != NULL) {
            Event e = current->data;

            cout << no << "   ";
            printPadded(e.getTitle(), 27);
            printPadded(e.getSubject(), 12);
            printPadded(e.getType(), 12);
            e.getDueDate().display();
            cout << "   " << e.getStatus(today) << "\n";

            current = current->next;
            no++;
        }
    }

    void displayCalendar() {
        if (head == NULL) {
            cout << "No events found.\n";
            return;
        }

        Node* current = head;
        long lastDate = -1;

        while (current != NULL) {
            long currentDate = current->data.getDueDate().toKey();

            if (currentDate != lastDate) {
                cout << "\n[ ";
                current->data.getDueDate().display();
                cout << " ]\n";
                lastDate = currentDate;
            }

            cout << "- " << current->data.getTitle()
                 << " [" << current->data.getType() << "] "
                 << current->data.getSubject() << "\n";

            current = current->next;
        }
    }

    void searchEvent(string keyword) {
        Node* current = head;
        bool found = false;

        while (current != NULL) {
            if (current->data.getTitle() == keyword ||
                current->data.getSubject() == keyword) {
                cout << "Found: " << current->data.getTitle()
                     << " | " << current->data.getSubject()
                     << " | ";
                current->data.getDueDate().display();
                cout << "\n";

                found = true;
            }

            current = current->next;
        }

        if (!found) {
            cout << "Event not found.\n";
        }
    }

    void markComplete(string title) {
        if (deleteEvent(title)) {
            cout << "Event marked complete and removed.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    void showReminders() {
        cout << "\nReminder Queue:\n";
        reminders.display();
    }

    void showOverdue() {
        cout << "\nOverdue Stack:\n";
        alerts.display();
    }

    void sortEvents(int choice, Date today) {
        if (head == NULL) {
            cout << "No events found.\n";
            return;
        }

        Event arr[100];
        int count = 0;

        Node* current = head;

        while (current != NULL && count < 100) {
            arr[count] = current->data;
            count++;
            current = current->next;
        }

        int i = 0;
        int j = 0;

        for (i = 0; i < count - 1; i++) {
            int selected = i;

            for (j = i + 1; j < count; j++) {
                bool choose = false;

                if (choice == 1) {
                    choose = arr[j].getDueDate().compareTo(arr[selected].getDueDate()) < 0;
                } else if (choice == 2) {
                    choose = arr[j].getSubject() < arr[selected].getSubject();
                } else if (choice == 3) {
                    choose = arr[j].statusRank(today) < arr[selected].statusRank(today);
                }

                if (choose) {
                    selected = j;
                }
            }

            if (selected != i) {
                Event temp = arr[i];
                arr[i] = arr[selected];
                arr[selected] = temp;
            }
        }

        cout << "No  Title                      Subject     Type        Due Date     Status\n";
        cout << "-------------------------------------------------------------------------------\n";

        for (i = 0; i < count; i++) {
            cout << i + 1 << "   ";
            printPadded(arr[i].getTitle(), 27);
            printPadded(arr[i].getSubject(), 12);
            printPadded(arr[i].getType(), 12);
            arr[i].getDueDate().display();
            cout << "   " << arr[i].getStatus(today) << "\n";
        }
    }

    void saveEventsToFile() {
        ofstream fout("events.txt");

        if (!fout) {
            cout << "Error: Cannot save events.txt.\n";
            return;
        }

        Node* current = head;

        while (current != NULL) {
            Event e = current->data;
            Date d = e.getDueDate();

            fout << e.getTitle() << ";"
                 << e.getSubject() << ";"
                 << e.getType() << ";"
                 << d.getDay() << ";"
                 << d.getMonth() << ";"
                 << d.getYear() << "\n";

            current = current->next;
        }

        fout.close();
    }
};

// ---------------------- Load from events.txt --------------------------
void loadEventsFromFile(Dashboard &dash) {
    ifstream fin("events.txt");

    if (!fin) {
        cout << "events.txt not found.\n";
        cout << "Please create events.txt in the same folder as main.cpp.\n";
        return;
    }

    string title = "";
    string subject = "";
    string type = "";
    string dayText = "";
    string monthText = "";
    string yearText = "";

    while (getline(fin, title, ';')) {
        getline(fin, subject, ';');
        getline(fin, type, ';');
        getline(fin, dayText, ';');
        getline(fin, monthText, ';');
        getline(fin, yearText);

        int day = stoi(dayText);
        int month = stoi(monthText);
        int year = stoi(yearText);

        dash.addEvent(Event(title, subject, type, Date(day, month, year)));
    }

    fin.close();

    cout << "Events loaded from events.txt.\n";
}

// ----------------------------- main ----------------------------------
int main() {
    Dashboard dash;
    Date today(6, 6, 2026);

    loadEventsFromFile(dash);

    int choice = 0;

    do {
       cout << "\n==========================================\n";
        cout << "          E-LEARNING DASHBOARD\n";
        cout << "==========================================\n";
        cout << " -- Retrieve Event Information --\n";
        cout << " 1. View Timeline (sorted by due date)\n";
        cout << " 2. View Calendar (grouped by date)\n";
        cout << " 3. Search Event\n";
        cout << " -- Update Event Information --\n";
        cout << " 4. Add Event\n";
        cout << " 5. Amend Event (change due date)\n";
        cout << " -- Manage Event Status --\n";
        cout << " 6. Mark Event Complete\n";
        cout << " 7. View Reminders (queue)\n";
        cout << " 8. View Overdue Alerts (stack)\n";
        cout << " -- Sort Event --\n";
        cout << " 9. Sort Events (date / subject / status)\n";
        cout << "10. Exit\n";
        cout << "------------------------------------------\n";
        cout << " Enter your choice (1-10): ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\n===== TIMELINE =====\n";
            dash.displayTimeline(today);

        } else if (choice == 2) {
            cout << "\n===== CALENDAR =====\n";
            dash.displayCalendar();

        } else if (choice == 3) {
            string keyword = "";

            cout << "Enter title or subject: ";
            getline(cin, keyword);

            dash.searchEvent(keyword);

        } else if (choice == 4) {
            string title = "";
            string subject = "";
            string type = "";
            int day = 0;
            int month = 0;
            int year = 0;

            cout << "Title: ";
            getline(cin, title);

            cout << "Subject: ";
            getline(cin, subject);

            cout << "Type: ";
            getline(cin, type);

            cout << "Due day: ";
            cin >> day;

            cout << "Due month: ";
            cin >> month;

            cout << "Due year: ";
            cin >> year;
            cin.ignore();

            dash.addEvent(Event(title, subject, type, Date(day, month, year)));
            dash.saveEventsToFile();

            cout << "Event added and saved to events.txt.\n";

        } else if (choice == 5) {
            string title = "";
            int day = 0;
            int month = 0;
            int year = 0;

            cout << "Enter event title to amend: ";
            getline(cin, title);

            cout << "New due day: ";
            cin >> day;

            cout << "New due month: ";
            cin >> month;

            cout << "New due year: ";
            cin >> year;
            cin.ignore();

            dash.amendEvent(title, Date(day, month, year));
            dash.saveEventsToFile();

            cout << "events.txt updated.\n";

        } else if (choice == 6) {
            string title = "";

            cout << "Enter event title to mark complete: ";
            getline(cin, title);

            dash.markComplete(title);
            dash.saveEventsToFile();

            cout << "events.txt updated.\n";

        } else if (choice == 7) {
            dash.showReminders();

        } else if (choice == 8) {
            dash.checkDeadlines(today);
            dash.showOverdue();

        } else if (choice == 9) {
            int sortChoice = 0;

            cout << "Sort by 1. Date  2. Subject  3. Status: ";
            cin >> sortChoice;
            cin.ignore();

            dash.sortEvents(sortChoice, today);

        } else if (choice == 10) {
            dash.saveEventsToFile();
            cout << "Events saved to events.txt. Goodbye!\n";

        } else {
            cout << "Invalid choice. Please enter 1 to 10.\n";
        }

    } while (choice != 10);

    return 0;
}