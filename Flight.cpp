// ============================================================
// Flight.cpp - Implementation of the Flight class
// ============================================================

#include "Flight.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Flight::Flight(const string& flightNumber,
    const string& origin,
    const string& destination,
    const string& date,
    const string& departureTime,
    const string& arrivalTime,
    int totalSeats,
    double basePrice)
    : flightNumber(flightNumber), origin(origin), destination(destination),
    date(date), departureTime(departureTime), arrivalTime(arrivalTime),
    totalSeats(totalSeats), basePrice(basePrice),
    seats(totalSeats, false)   // All seats start as available
{
}

// ── Getters ──────────────────────────────────────────────────
string Flight::getFlightNumber()  const { return flightNumber; }
string Flight::getOrigin()        const { return origin; }
string Flight::getDestination()   const { return destination; }
string Flight::getDate()          const { return date; }
string Flight::getDepartureTime() const { return departureTime; }
string Flight::getArrivalTime()   const { return arrivalTime; }
int    Flight::getTotalSeats()    const { return totalSeats; }
double Flight::getBasePrice()     const { return basePrice; }

int Flight::getAvailableSeats() const {
    int count = 0;
    for (bool occupied : seats)
        if (!occupied) ++count;
    return count;
}

// ── Seat Operations ───────────────────────────────────────────
bool Flight::isSeatAvailable(int seatNum) const {
    if (seatNum < 1 || seatNum > totalSeats) return false;
    return !seats[seatNum - 1];
}

bool Flight::bookSeat(int seatNum) {
    if (!isSeatAvailable(seatNum)) return false;
    seats[seatNum - 1] = true;
    return true;
}

bool Flight::cancelSeat(int seatNum) {
    if (seatNum < 1 || seatNum > totalSeats) return false;
    seats[seatNum - 1] = false;
    return true;
}

vector<int> Flight::getAvailableSeatNumbers() const {
    vector<int> available;
    for (int i = 0; i < totalSeats; ++i)
        if (!seats[i]) available.push_back(i + 1);
    return available;
}

// ── Display ───────────────────────────────────────────────────
void Flight::displaySummary() const {
    cout << left
        << "  Flight  : " << flightNumber << "\n"
        << "  Route   : " << origin << "  -->  " << destination << "\n"
        << "  Date    : " << date << "\n"
        << "  Depart  : " << departureTime << "   Arrive: " << arrivalTime << "\n"
        << "  Seats   : " << getAvailableSeats() << " / " << totalSeats << " available\n"
        << "  Price   : $" << fixed << setprecision(2) << basePrice << "\n";
}

void Flight::displaySeatMap() const {
    cout << "\n  Seat Map for Flight " << flightNumber
        << "  [O=Available  X=Booked]\n";
    cout << "  ";
    for (int i = 0; i < totalSeats; ++i) {
        if (i % 6 == 0 && i != 0) cout << "\n  ";
        cout << setw(3) << (i + 1) << ":"
            << (seats[i] ? "X" : "O") << " ";
    }
    cout << "\n";
}

// ── Serialization ─────────────────────────────────────────────
// Format: flightNumber|origin|destination|date|depTime|arrTime|totalSeats|price|seatBitmask
string Flight::serialize() const {
    ostringstream ss;
    ss << flightNumber << "|" << origin << "|" << destination << "|"
        << date << "|" << departureTime << "|" << arrivalTime << "|"
        << totalSeats << "|" << basePrice << "|";
    // Encode seat occupancy as a string of 0/1
    for (bool s : seats) ss << (s ? '1' : '0');
    return ss.str();
}

Flight Flight::deserialize(const string& line) {
    istringstream ss(line);
    string fn, ori, dest, dt, dep, arr, tsStr, prStr, seatStr;
    getline(ss, fn, '|');
    getline(ss, ori, '|');
    getline(ss, dest, '|');
    getline(ss, dt, '|');
    getline(ss, dep, '|');
    getline(ss, arr, '|');
    getline(ss, tsStr, '|');
    getline(ss, prStr, '|');
    getline(ss, seatStr);

    int totalSeats = stoi(tsStr);
    double price = stod(prStr);

    Flight f(fn, ori, dest, dt, dep, arr, totalSeats, price);

    // Restore seat occupancy
    for (int i = 0; i < (int)seatStr.size() && i < totalSeats; ++i)
        f.seats[i] = (seatStr[i] == '1');

    return f;
}