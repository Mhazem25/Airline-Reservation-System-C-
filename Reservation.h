#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include "Passenger.h"

// ============================================================
// Reservation.h - Links a Passenger to a Flight + seat
// ============================================================

using namespace std;

enum class SeatClass { ECONOMY, BUSINESS, FIRST };

class Reservation {
private:
    string   reservationID;
    Passenger passenger;
    string   flightNumber;
    int      seatNumber;
    SeatClass seatClass;
    double   pricePaid;
    string   bookingDate;   // YYYY-MM-DD
    bool     active;        // false if cancelled

public:
    // Constructors
    Reservation() = default;
    Reservation(const string& resID,
        const Passenger& passenger,
        const string& flightNumber,
        int seatNumber,
        SeatClass seatClass,
        double pricePaid,
        const string& bookingDate);

    // Getters
    string    getReservationID() const;
    Passenger getPassenger()     const;
    string    getFlightNumber()  const;
    int       getSeatNumber()    const;
    SeatClass getSeatClass()     const;
    double    getPricePaid()     const;
    string    getBookingDate()   const;
    bool      isActive()         const;

    // Cancel this reservation
    void cancel();

    // Display
    void display() const;

    // Serialization
    string           serialize() const;
    static Reservation deserialize(const string& line);

    // Helper: class name string
    static string   className(SeatClass sc);
    static SeatClass classFromString(const string& s);
};

#endif // RESERVATION_H