#ifndef AIRLINESYSTEM_H
#define AIRLINESYSTEM_H

#include <map>
#include <vector>
#include <string>
#include "Flight.h"
#include "Passenger.h"
#include "Reservation.h"
#include "User.h"

using namespace std;

// ============================================================
// AirlineSystem.h - Central manager for all system operations
// ============================================================

class AirlineSystem {
private:
    // In-memory stores (key = primary identifier)
    map<string, Flight>      flights;       // key: flightNumber
    map<string, Passenger>   passengers;    // key: passengerID
    map<string, Reservation> reservations;  // key: reservationID
    map<string, User>        users;         // key: username

    // Currently logged-in user (empty string = not logged in)
    string currentUsername;

    // Counter for generating unique IDs
    int reservationCounter;

    // File paths
    static const string FLIGHTS_FILE;
    static const string RESERVATIONS_FILE;
    static const string PASSENGERS_FILE;
    static const string USERS_FILE;

public:
    AirlineSystem();

    // ── Authentication ────────────────────────────────────────
    bool   login(const string& username, const string& password);
    void   logout();
    bool   isLoggedIn() const;
    bool   isAdmin()    const;
    string getCurrentUsername() const;

    // ── Flight Management (admin) ─────────────────────────────
    bool   addFlight(const Flight& flight);
    bool   removeFlight(const string& flightNumber);
    Flight* findFlight(const string& flightNumber);

    // ── Search Flights ────────────────────────────────────────
    vector<Flight*> searchByDestination(const string& dest);
    vector<Flight*> searchByDate(const string& date);
    vector<Flight*> searchByRoute(const string& origin,
        const string& dest);
    void            listAllFlights() const;

    // ── Booking ───────────────────────────────────────────────
    string bookTicket(const string& flightNumber,
        const Passenger& passenger,
        int seatNumber,
        SeatClass seatClass);

    bool cancelReservation(const string& reservationID);

    // ── Passenger / Reservation Queries ───────────────────────
    Reservation* findReservation(const string& reservationID);
    vector<Reservation*>    getReservationsForPassenger(const string& passengerID);
    void                    listAllReservations() const;

    // ── User Management (admin) ───────────────────────────────
    bool registerUser(const string& username,
        const string& password,
        UserRole role,
        const string& linkedPassengerID = "");

    // ── Persistence ───────────────────────────────────────────
    void saveAll() const;
    void loadAll();

private:
    // Helpers
    string today() const;                 // Returns current date as YYYY-MM-DD
    string generateReservationID();
    double calculatePrice(double basePrice, SeatClass sc) const;

    void saveFlights()      const;
    void saveReservations() const;
    void savePassengers()   const;
    void saveUsers()        const;

    void loadFlights();
    void loadReservations();
    void loadPassengers();
    void loadUsers();
    void seedDefaultData();  // Populate initial sample data if files are empty
};

#endif // AIRLINESYSTEM_H