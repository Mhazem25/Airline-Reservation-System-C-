// ============================================================
// AirlineSystem.cpp - Implementation of AirlineSystem
// ============================================================

#include "AirlineSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

// ── File path constants ───────────────────────────────────────
const string AirlineSystem::FLIGHTS_FILE = "data/flights.dat";
const string AirlineSystem::RESERVATIONS_FILE = "data/reservations.dat";
const string AirlineSystem::PASSENGERS_FILE = "data/passengers.dat";
const string AirlineSystem::USERS_FILE = "data/users.dat";

// ── Constructor ───────────────────────────────────────────────
AirlineSystem::AirlineSystem() : reservationCounter(1000) {
    loadAll();
}

// ── Authentication ────────────────────────────────────────────
bool AirlineSystem::login(const string& username, const string& password) {
    auto it = users.find(username);
    if (it == users.end()) return false;
    if (!it->second.checkPassword(password)) return false;
    currentUsername = username;
    return true;
}

void AirlineSystem::logout() { currentUsername.clear(); }
bool AirlineSystem::isLoggedIn() const { return !currentUsername.empty(); }
bool AirlineSystem::isAdmin() const {
    auto it = users.find(currentUsername);
    return it != users.end() && it->second.isAdmin();
}
string AirlineSystem::getCurrentUsername() const { return currentUsername; }

// ── Flight Management ─────────────────────────────────────────
bool AirlineSystem::addFlight(const Flight& flight) {
    if (flights.count(flight.getFlightNumber())) return false;  // duplicate
    flights[flight.getFlightNumber()] = flight;
    saveFlights();
    return true;
}

bool AirlineSystem::removeFlight(const string& flightNumber) {
    auto it = flights.find(flightNumber);
    if (it == flights.end()) return false;
    flights.erase(it);
    saveFlights();
    return true;
}

Flight* AirlineSystem::findFlight(const string& flightNumber) {
    auto it = flights.find(flightNumber);
    return (it != flights.end()) ? &it->second : nullptr;
}

// ── Search ────────────────────────────────────────────────────
vector<Flight*> AirlineSystem::searchByDestination(const string& dest) {
    vector<Flight*> result;
    for (auto& [key, f] : flights) {
        string d = f.getDestination();
        string q = dest;
        // Case-insensitive substring match
        transform(d.begin(), d.end(), d.begin(), ::tolower);
        transform(q.begin(), q.end(), q.begin(), ::tolower);
        if (d.find(q) != string::npos)
            result.push_back(&f);
    }
    return result;
}

vector<Flight*> AirlineSystem::searchByDate(const string& date) {
    vector<Flight*> result;
    for (auto& [key, f] : flights)
        if (f.getDate() == date) result.push_back(&f);
    return result;
}

vector<Flight*> AirlineSystem::searchByRoute(const string& origin,
    const string& dest) {
    vector<Flight*> result;
    for (auto& [key, f] : flights) {
        string o = f.getOrigin(), d = f.getDestination();
        string oq = origin, dq = dest;
        transform(o.begin(), o.end(), o.begin(), ::tolower);
        transform(d.begin(), d.end(), d.begin(), ::tolower);
        transform(oq.begin(), oq.end(), oq.begin(), ::tolower);
        transform(dq.begin(), dq.end(), dq.begin(), ::tolower);
        if (o.find(oq) != string::npos && d.find(dq) != string::npos)
            result.push_back(&f);
    }
    return result;
}

void AirlineSystem::listAllFlights() const {
    if (flights.empty()) {
        cout << "  No flights available.\n";
        return;
    }
    for (const auto& [key, f] : flights) {
        cout << "  ----------------------------------------\n";
        f.displaySummary();
    }
    cout << "  ----------------------------------------\n";
}

// ── Booking ───────────────────────────────────────────────────
string AirlineSystem::bookTicket(const string& flightNumber,
    const Passenger& passenger,
    int seatNumber,
    SeatClass seatClass) {
    Flight* flight = findFlight(flightNumber);
    if (!flight) return "";

    if (!flight->bookSeat(seatNumber)) return "";   // Seat unavailable

    // Store passenger if new
    passengers[passenger.getPassengerID()] = passenger;

    double price = calculatePrice(flight->getBasePrice(), seatClass);
    string resID = generateReservationID();

    Reservation res(resID, passenger, flightNumber, seatNumber, seatClass, price, today());
    reservations[resID] = res;

    saveAll();
    return resID;
}

bool AirlineSystem::cancelReservation(const string& reservationID) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end() || !it->second.isActive()) return false;

    // Free the seat on the flight
    Flight* flight = findFlight(it->second.getFlightNumber());
    if (flight) flight->cancelSeat(it->second.getSeatNumber());

    it->second.cancel();
    saveAll();
    return true;
}

// ── Queries ───────────────────────────────────────────────────
Reservation* AirlineSystem::findReservation(const string& reservationID) {
    auto it = reservations.find(reservationID);
    return (it != reservations.end()) ? &it->second : nullptr;
}

vector<Reservation*> AirlineSystem::getReservationsForPassenger(
    const string& passengerID) {
    vector<Reservation*> result;
    for (auto& [key, r] : reservations)
        if (r.getPassenger().getPassengerID() == passengerID)
            result.push_back(&r);
    return result;
}

void AirlineSystem::listAllReservations() const {
    if (reservations.empty()) {
        cout << "  No reservations found.\n";
        return;
    }
    for (const auto& [key, r] : reservations) {
        cout << "  ----------------------------------------\n";
        r.display();
    }
    cout << "  ----------------------------------------\n";
}

// ── User Management ───────────────────────────────────────────
bool AirlineSystem::registerUser(const string& username,
    const string& password,
    UserRole role,
    const string& linkedPassengerID) {
    if (users.count(username)) return false;  // duplicate
    users[username] = User(username, password, role, linkedPassengerID);
    saveUsers();
    return true;
}

// ── Persistence ───────────────────────────────────────────────
void AirlineSystem::saveAll() const {
    saveFlights();
    saveReservations();
    savePassengers();
    saveUsers();
}

void AirlineSystem::loadAll() {
    loadUsers();
    loadFlights();
    loadPassengers();
    loadReservations();

    // If no users at all, seed default data
    if (users.empty()) seedDefaultData();
}

void AirlineSystem::saveFlights() const {
    ofstream file(FLIGHTS_FILE);
    for (const auto& [key, f] : flights)
        file << f.serialize() << "\n";
}

void AirlineSystem::saveReservations() const {
    ofstream file(RESERVATIONS_FILE);
    for (const auto& [key, r] : reservations)
        file << r.serialize() << "\n";
}

void AirlineSystem::savePassengers() const {
    ofstream file(PASSENGERS_FILE);
    for (const auto& [key, p] : passengers)
        file << p.serialize() << "\n";
}

void AirlineSystem::saveUsers() const {
    ofstream file(USERS_FILE);
    for (const auto& [key, u] : users)
        file << u.serialize() << "\n";
}

void AirlineSystem::loadFlights() {
    ifstream file(FLIGHTS_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line))
        if (!line.empty()) {
            Flight f = Flight::deserialize(line);
            flights[f.getFlightNumber()] = f;
        }
}

void AirlineSystem::loadReservations() {
    ifstream file(RESERVATIONS_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line))
        if (!line.empty()) {
            Reservation r = Reservation::deserialize(line);
            reservations[r.getReservationID()] = r;
            // Update reservation counter to stay above existing IDs
            try {
                int num = stoi(r.getReservationID().substr(3));
                if (num >= reservationCounter) reservationCounter = num + 1;
            }
            catch (...) {}
        }
}

void AirlineSystem::loadPassengers() {
    ifstream file(PASSENGERS_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line))
        if (!line.empty()) {
            Passenger p = Passenger::deserialize(line);
            passengers[p.getPassengerID()] = p;
        }
}

void AirlineSystem::loadUsers() {
    ifstream file(USERS_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line))
        if (!line.empty()) {
            User u = User::deserialize(line);
            users[u.getUsername()] = u;
        }
}

// ── Seed Data ─────────────────────────────────────────────────
void AirlineSystem::seedDefaultData() {
    // Default admin account
    registerUser("admin", "admin123", UserRole::ADMIN);
    // Default user account (linked to a sample passenger)
    registerUser("john_doe", "pass123", UserRole::USER, "P001");

    // Sample flights
    addFlight(Flight("EK101", "Dubai", "London", "2025-08-15", "08:00", "12:30", 30, 450.00));
    addFlight(Flight("EK202", "Dubai", "New York", "2025-08-16", "10:00", "19:45", 24, 780.00));
    addFlight(Flight("EK303", "Cairo", "Dubai", "2025-08-17", "14:00", "18:00", 20, 220.00));
    addFlight(Flight("EK404", "London", "Paris", "2025-08-18", "07:00", "09:15", 18, 150.00));
    addFlight(Flight("EK505", "New York", "Cairo", "2025-08-20", "23:00", "17:00", 28, 920.00));
    addFlight(Flight("EK606", "Cairo", "Istanbul", "2025-08-22", "06:30", "09:00", 22, 310.00));

    // Sample passenger
    Passenger p("P001", "John Doe", "john@email.com", "+201001234567");
    passengers["P001"] = p;
    savePassengers();

    cout << "\n  [System] Sample data loaded. Default credentials:\n"
        << "    Admin  -> username: admin    / password: admin123\n"
        << "    User   -> username: john_doe / password: pass123\n\n";
}

// ── Helpers ───────────────────────────────────────────────────
string AirlineSystem::generateReservationID() {
    ostringstream ss;
    ss << "RES" << setw(6) << setfill('0') << reservationCounter++;
    return ss.str();
}

double AirlineSystem::calculatePrice(double basePrice, SeatClass sc) const {
    switch (sc) {
    case SeatClass::ECONOMY:  return basePrice;
    case SeatClass::BUSINESS: return basePrice * 2.5;
    case SeatClass::FIRST:    return basePrice * 4.0;
    }
    return basePrice;
}

string AirlineSystem::today() const {
    time_t t = time(nullptr);
    tm tmStruct; // Local struct instead of a pointer

    // MSVC version of localtime_s (returns 0 on success)
    localtime_s(&tmStruct, &t);  // fills tmStruct with local time [web:34]

    ostringstream ss;
    ss << (1900 + tmStruct.tm_year) << "-"
        << setw(2) << setfill('0') << (tmStruct.tm_mon + 1) << "-"
        << setw(2) << setfill('0') << tmStruct.tm_mday;
    return ss.str();
}