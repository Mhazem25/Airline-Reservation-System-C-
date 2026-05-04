// ============================================================
// Menu.cpp - Console menu and UI implementation
// ============================================================
#include "Menu.h"
#include <iostream>
#include <limits>
#include <regex>
#include <iomanip>
using namespace std;
namespace Menu {

    // ── Utility helpers ───────────────────────────────────────────
    void printBanner() {
        cout << "\n";
        printLine('=', 58);
        cout << "   ✈   SKYLINK AIRLINE RESERVATION SYSTEM   ✈\n";
        printLine('=', 58);
        cout << "\n";
    }

    void printLine(char c, int width) {
        cout << string(width, c) << "\n";
    }

    void pause() {
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string getInput(const string& prompt) {
        cout << "  " << prompt;
        string val;
        getline(cin, val);
        // Trim leading/trailing whitespace
        val.erase(0, val.find_first_not_of(" \t"));
        val.erase(val.find_last_not_of(" \t") + 1);
        return val;
    }

    int getIntInput(const string& prompt, int minVal, int maxVal) {
        while (true) {
            string raw = getInput(prompt);
            try {
                int val = stoi(raw);
                if (val >= minVal && val <= maxVal) return val;
            }
            catch (...) {}
            cout << "  [!] Please enter a number between "
                << minVal << " and " << maxVal << ".\n";
        }
    }

    double getDoubleInput(const string& prompt) {
        while (true) {
            string raw = getInput(prompt);
            try { return stod(raw); }
            catch (...) {}
            cout << "  [!] Please enter a valid number.\n";
        }
    }

    // ── Validation ────────────────────────────────────────────────
    bool isValidDate(const string& date) {
        regex r(R"(\d{4}-\d{2}-\d{2})");
        return regex_match(date, r);
    }

    bool isValidTime(const string& time) {
        regex r(R"(\d{2}:\d{2})");
        return regex_match(time, r);
    }

    bool isValidEmail(const string& email) {
        regex r(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
        return regex_match(email, r);
    }

    bool isValidPhone(const string& phone) {
        regex r(R"(\+?[0-9\s\-]{7,15})");
        return regex_match(phone, r);
    }

    // ── Login ─────────────────────────────────────────────────────
    void loginMenu(AirlineSystem& sys) {
        printBanner();
        cout << "  [ LOGIN ]\n\n";
        string username = getInput("Username: ");
        // Password (echoed for simplicity; cin.get would suppress echo on Unix)
        string password = getInput("Password: ");

        if (sys.login(username, password)) {
            cout << "\n  Welcome, " << username << "!\n";
            pause();
            if (sys.isAdmin())
                adminMainMenu(sys);
            else
                userMainMenu(sys);
        }
        else {
            cout << "\n  [!] Invalid credentials. Please try again.\n";
            pause();
        }
    }

    // ── Admin Main Menu ───────────────────────────────────────────
    void adminMainMenu(AirlineSystem& sys) {
        while (true) {
            printBanner();
            cout << "  ADMIN MENU  [ Logged in as: "
                << sys.getCurrentUsername() << " ]\n\n"
                << "  1. Manage Flights\n"
                << "  2. All Reservations\n"
                << "  3. Manage Users\n"
                << "  4. Logout\n\n";

            int choice = getIntInput("Select option: ", 1, 4);
            switch (choice) {
            case 1: manageFlightsMenu(sys); break;
            case 2: manageReservationsMenu(sys); break;
            case 3: manageUsersMenu(sys); break;
            case 4: sys.logout(); return;
            }
        }
    }

    // ── User Main Menu ────────────────────────────────────────────
    void userMainMenu(AirlineSystem& sys) {
        while (true) {
            printBanner();
            cout << "  PASSENGER MENU  [ Logged in as: "
                << sys.getCurrentUsername() << " ]\n\n"
                << "  1. Search Flights\n"
                << "  2. Book a Ticket\n"
                << "  3. My Reservations\n"
                << "  4. Cancel a Reservation\n"
                << "  5. Logout\n\n";

            int choice = getIntInput("Select option: ", 1, 5);
            switch (choice) {
            case 1: searchFlightsMenu(sys); break;
            case 2: bookTicketMenu(sys); break;
            case 3: myReservationsMenu(sys); break;
            case 4: {
                string resID = getInput("Reservation ID to cancel: ");
                if (sys.cancelReservation(resID))
                    cout << "\n  [✔] Reservation " << resID << " cancelled.\n";
                else
                    cout << "\n  [!] Reservation not found or already cancelled.\n";
                pause();
                break;
            }
            case 5: sys.logout(); return;
            }
        }
    }

    // ── Manage Flights (Admin) ────────────────────────────────────
    void manageFlightsMenu(AirlineSystem& sys) {
        while (true) {
            printBanner();
            cout << "  FLIGHT MANAGEMENT\n\n"
                << "  1. List All Flights\n"
                << "  2. Add New Flight\n"
                << "  3. Remove Flight\n"
                << "  4. View Seat Map\n"
                << "  5. Back\n\n";

            int choice = getIntInput("Select option: ", 1, 5);

            if (choice == 5) return;

            if (choice == 1) {
                printLine();
                sys.listAllFlights();
                pause();

            }
            else if (choice == 2) {
                printLine();
                cout << "  ADD NEW FLIGHT\n";
                printLine();

                string fn, ori, dest, date, dep, arr;
                fn = getInput("Flight Number (e.g., EK707): ");

                ori = getInput("Origin City: ");
                while (ori.empty()) {
                    cout << "  [!] Origin cannot be empty.\n";
                    ori = getInput("Origin City: ");
                }
                dest = getInput("Destination City: ");
                while (dest.empty()) {
                    cout << "  [!] Destination cannot be empty.\n";
                    dest = getInput("Destination City: ");
                }

                date = getInput("Date (YYYY-MM-DD): ");
                while (!isValidDate(date)) {
                    cout << "  [!] Invalid date format.\n";
                    date = getInput("Date (YYYY-MM-DD): ");
                }
                dep = getInput("Departure Time (HH:MM): ");
                while (!isValidTime(dep)) {
                    cout << "  [!] Invalid time format.\n";
                    dep = getInput("Departure Time (HH:MM): ");
                }
                arr = getInput("Arrival Time (HH:MM): ");
                while (!isValidTime(arr)) {
                    cout << "  [!] Invalid time format.\n";
                    arr = getInput("Arrival Time (HH:MM): ");
                }

                int seats = getIntInput("Total Seats (1-500): ", 1, 500);
                double price = getDoubleInput("Base Economy Price (USD): ");

                Flight f(fn, ori, dest, date, dep, arr, seats, price);
                if (sys.addFlight(f))
                    cout << "\n  [✔] Flight " << fn << " added.\n";
                else
                    cout << "\n  [!] Flight number already exists.\n";
                pause();

            }
            else if (choice == 3) {
                string fn = getInput("Flight Number to remove: ");
                if (sys.removeFlight(fn))
                    cout << "\n  [✔] Flight removed.\n";
                else
                    cout << "\n  [!] Flight not found.\n";
                pause();

            }
            else if (choice == 4) {
                string fn = getInput("Flight Number: ");
                Flight* f = sys.findFlight(fn);
                if (f) f->displaySeatMap();
                else   cout << "\n  [!] Flight not found.\n";
                pause();
            }
        }
    }

    // ── Manage Reservations (Admin) ───────────────────────────────
    void manageReservationsMenu(AirlineSystem& sys) {
        while (true) {
            printBanner();
            cout << "  ALL RESERVATIONS\n\n"
                << "  1. List All Reservations\n"
                << "  2. View Specific Reservation\n"
                << "  3. Cancel a Reservation\n"
                << "  4. Back\n\n";

            int choice = getIntInput("Select option: ", 1, 4);
            if (choice == 4) return;

            if (choice == 1) {
                printLine();
                sys.listAllReservations();
                pause();
            }
            else if (choice == 2) {
                string id = getInput("Reservation ID: ");
                Reservation* r = sys.findReservation(id);
                if (r) { printLine(); r->display(); }
                else   cout << "\n  [!] Not found.\n";
                pause();
            }
            else if (choice == 3) {
                string id = getInput("Reservation ID to cancel: ");
                if (sys.cancelReservation(id))
                    cout << "\n  [✔] Cancelled.\n";
                else
                    cout << "\n  [!] Not found or already cancelled.\n";
                pause();
            }
        }
    }

    // ── Manage Users (Admin) ──────────────────────────────────────
    void manageUsersMenu(AirlineSystem& sys) {
        printBanner();
        cout << "  CREATE NEW USER\n";
        printLine();

        string uname = getInput("New Username: ");
        string pwd = getInput("Password: ");
        string pwd2 = getInput("Confirm Password: ");
        if (pwd != pwd2) {
            cout << "\n  [!] Passwords do not match.\n";
            pause();
            return;
        }

        cout << "  Role: 1=Admin  2=User\n";
        int roleChoice = getIntInput("Select role: ", 1, 2);
        UserRole role = (roleChoice == 1) ? UserRole::ADMIN : UserRole::USER;

        string linked;
        if (role == UserRole::USER)
            linked = getInput("Linked Passenger ID (or blank): ");

        if (sys.registerUser(uname, pwd, role, linked))
            cout << "\n  [✔] User '" << uname << "' created.\n";
        else
            cout << "\n  [!] Username already exists.\n";
        pause();
    }

    // ── Search Flights (User) ─────────────────────────────────────
    void searchFlightsMenu(AirlineSystem& sys) {
        printBanner();
        cout << "  SEARCH FLIGHTS\n\n"
            << "  1. Search by Destination\n"
            << "  2. Search by Date\n"
            << "  3. Search by Route\n"
            << "  4. List All Flights\n"
            << "  5. Back\n\n";

        int choice = getIntInput("Select option: ", 1, 5);
        if (choice == 5) return;

        vector<Flight*> results;

        if (choice == 1) {
            string dest = getInput("Destination: ");
            results = sys.searchByDestination(dest);
        }
        else if (choice == 2) {
            string date = getInput("Date (YYYY-MM-DD): ");
            results = sys.searchByDate(date);
        }
        else if (choice == 3) {
            string ori = getInput("Origin: ");
            string dest = getInput("Destination: ");
            results = sys.searchByRoute(ori, dest);
        }
        else {
            sys.listAllFlights();
            pause();
            return;
        }

        printLine();
        if (results.empty()) {
            cout << "  No flights found.\n";
        }
        else {
            cout << "  Found " << results.size() << " flight(s):\n";
            for (Flight* f : results) {
                printLine('-', 40);
                f->displaySummary();
            }
        }
        pause();
    }

    // ── Book Ticket (User) ────────────────────────────────────────
    void bookTicketMenu(AirlineSystem& sys) {
        printBanner();
        cout << "  BOOK A TICKET\n";
        printLine();

        // Step 1: Select flight
        string fn = getInput("Flight Number: ");
        Flight* flight = sys.findFlight(fn);
        if (!flight) {
            cout << "\n  [!] Flight not found.\n";
            pause();
            return;
        }
        flight->displaySummary();
        if (flight->getAvailableSeats() == 0) {
            cout << "\n  [!] No seats available on this flight.\n";
            pause();
            return;
        }
        flight->displaySeatMap();

        // Step 2: Choose seat
        int seatNum = getIntInput("Seat Number: ", 1, flight->getTotalSeats());
        if (!flight->isSeatAvailable(seatNum)) {
            cout << "\n  [!] Seat " << seatNum << " is not available.\n";
            pause();
            return;
        }

        // Step 3: Choose class
        cout << "\n  Class options:\n"
            << "    1. Economy  (base price)\n"
            << "    2. Business (x2.5)\n"
            << "    3. First    (x4.0)\n";
        int classChoice = getIntInput("Select class: ", 1, 3);
        SeatClass sc = (classChoice == 1) ? SeatClass::ECONOMY
            : (classChoice == 2) ? SeatClass::BUSINESS
            : SeatClass::FIRST;

        // Step 4: Passenger info
        printLine();
        cout << "  PASSENGER DETAILS\n";

        string pid = getInput("Passenger ID (e.g., P002): ");
        string name = getInput("Full Name: ");
        while (name.empty()) {
            cout << "  [!] Name cannot be empty.\n";
            name = getInput("Full Name: ");
        }

        string email = getInput("Email: ");
        while (!isValidEmail(email)) {
            cout << "  [!] Invalid email.\n";
            email = getInput("Email: ");
        }

        string phone = getInput("Phone: ");
        while (!isValidPhone(phone)) {
            cout << "  [!] Invalid phone number.\n";
            phone = getInput("Phone: ");
        }

        Passenger p(pid, name, email, phone);

        // Step 5: Confirm and book
        double price = flight->getBasePrice()
            * (sc == SeatClass::ECONOMY ? 1.0
                : sc == SeatClass::BUSINESS ? 2.5 : 4.0);
        cout << "\n  ── Booking Summary ──────────────────────\n"
            << "  Flight  : " << fn << "\n"
            << "  Seat    : " << seatNum << "\n"
            << "  Class   : " << Reservation::className(sc) << "\n"
            << "  Price   : $" << fixed << setprecision(2) << price << "\n"
            << "  Passenger: " << name << "\n"
            << "  ─────────────────────────────────────────\n";

        string confirm = getInput("Confirm booking? (y/n): ");
        if (confirm != "y" && confirm != "Y") {
            cout << "  Booking cancelled.\n";
            pause();
            return;
        }

        string resID = sys.bookTicket(fn, p, seatNum, sc);
        if (!resID.empty())
            cout << "\n  [✔] Booking successful! Reservation ID: " << resID << "\n";
        else
            cout << "\n  [!] Booking failed. Seat may have been taken.\n";
        pause();
    }

    // ── My Reservations (User) ────────────────────────────────────
    void myReservationsMenu(AirlineSystem& sys) {
        printBanner();
        cout << "  MY RESERVATIONS\n\n";
        string pid = getInput("Enter your Passenger ID: ");
        auto list = sys.getReservationsForPassenger(pid);

        if (list.empty()) {
            cout << "\n  No reservations found for ID: " << pid << "\n";
        }
        else {
            for (Reservation* r : list) {
                printLine();
                r->display();
            }
        }
        pause();
    }

} // namespace Menu