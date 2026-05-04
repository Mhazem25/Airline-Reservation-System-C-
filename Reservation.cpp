// ============================================================
// Reservation.cpp - Implementation of the Reservation class
// ============================================================

#include "Reservation.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Reservation::Reservation(const string& resID,
    const Passenger& passenger,
    const string& flightNumber,
    int seatNumber,
    SeatClass seatClass,
    double pricePaid,
    const string& bookingDate)
    : reservationID(resID), passenger(passenger),
    flightNumber(flightNumber), seatNumber(seatNumber),
    seatClass(seatClass), pricePaid(pricePaid),
    bookingDate(bookingDate), active(true) {
}

// ── Getters ──────────────────────────────────────────────────
string   Reservation::getReservationID() const { return reservationID; }
Passenger Reservation::getPassenger()    const { return passenger; }
string   Reservation::getFlightNumber()  const { return flightNumber; }
int      Reservation::getSeatNumber()    const { return seatNumber; }
SeatClass Reservation::getSeatClass()    const { return seatClass; }
double   Reservation::getPricePaid()     const { return pricePaid; }
string   Reservation::getBookingDate()   const { return bookingDate; }
bool     Reservation::isActive()         const { return active; }

void Reservation::cancel() { active = false; }

// ── Display ───────────────────────────────────────────────────
void Reservation::display() const {
    cout << "  Reservation ID : " << reservationID << "\n"
        << "  Status         : " << (active ? "ACTIVE" : "CANCELLED") << "\n"
        << "  Flight         : " << flightNumber << "\n"
        << "  Seat           : " << seatNumber
        << "  (" << className(seatClass) << ")\n"
        << "  Price Paid     : $" << fixed << setprecision(2) << pricePaid << "\n"
        << "  Booked On      : " << bookingDate << "\n";
    passenger.display();
}

// ── Helpers ───────────────────────────────────────────────────
string Reservation::className(SeatClass sc) {
    switch (sc) {
    case SeatClass::ECONOMY:  return "Economy";
    case SeatClass::BUSINESS: return "Business";
    case SeatClass::FIRST:    return "First";
    }
    return "Economy";
}

SeatClass Reservation::classFromString(const string& s) {
    if (s == "Business") return SeatClass::BUSINESS;
    if (s == "First")    return SeatClass::FIRST;
    return SeatClass::ECONOMY;
}

// ── Serialization ─────────────────────────────────────────────
// Format: resID|active|flightNum|seatNum|class|price|bookDate|passengerSerialized
string Reservation::serialize() const {
    ostringstream ss;
    ss << reservationID << "|"
        << (active ? "1" : "0") << "|"
        << flightNumber << "|"
        << seatNumber << "|"
        << className(seatClass) << "|"
        << fixed << setprecision(2) << pricePaid << "|"
        << bookingDate << "|"
        << passenger.serialize();
    return ss.str();
}

Reservation Reservation::deserialize(const string& line) {
    // Split on first 7 pipes; remainder is passenger data
    istringstream ss(line);
    string resID, activeStr, flightNum, seatStr, cls, priceStr, bookDate, passengerData;
    getline(ss, resID, '|');
    getline(ss, activeStr, '|');
    getline(ss, flightNum, '|');
    getline(ss, seatStr, '|');
    getline(ss, cls, '|');
    getline(ss, priceStr, '|');
    getline(ss, bookDate, '|');
    getline(ss, passengerData);   // rest of line

    Passenger p = Passenger::deserialize(passengerData);
    Reservation r(resID, p, flightNum,
        stoi(seatStr),
        classFromString(cls),
        stod(priceStr),
        bookDate);
    r.active = (activeStr == "1");
    return r;
}