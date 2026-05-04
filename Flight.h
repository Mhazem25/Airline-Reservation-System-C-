#ifndef FLIGHT_H
#define FLIGHT_H
using namespace std;
#include <string>
#include <vector>

// ============================================================
// Flight.h - Represents a single flight with seat management
// ============================================================

class Flight {
private:
    std::string flightNumber;   // e.g., "EK101"
    std::string origin;
    std::string destination;
    std::string date;           // Format: YYYY-MM-DD
    std::string departureTime;  // Format: HH:MM
    std::string arrivalTime;    // Format: HH:MM
    int totalSeats;
    double basePrice;           // Base ticket price in USD

    // Seat map: true = occupied, false = available
    std::vector<bool> seats;

public:
    // Constructors
    Flight() = default;
    Flight(const std::string& flightNumber,
        const std::string& origin,
        const std::string& destination,
        const std::string& date,
        const std::string& departureTime,
        const std::string& arrivalTime,
        int totalSeats,
        double basePrice);

    // Getters
    std::string getFlightNumber()  const;
    std::string getOrigin()        const;
    std::string getDestination()   const;
    std::string getDate()          const;
    std::string getDepartureTime() const;
    std::string getArrivalTime()   const;
    int         getTotalSeats()    const;
    int         getAvailableSeats()const;
    double      getBasePrice()     const;

    // Seat operations
    bool isSeatAvailable(int seatNum) const;  // seatNum is 1-based
    bool bookSeat(int seatNum);
    bool cancelSeat(int seatNum);
    std::vector<int> getAvailableSeatNumbers() const;

    // Display helpers
    void displaySummary()  const;
    void displaySeatMap()  const;

    // Serialization
    std::string serialize() const;
    static Flight deserialize(const std::string& line);
};

#endif // FLIGHT_H