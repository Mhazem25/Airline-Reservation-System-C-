#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

// ============================================================
// Passenger.h - Represents a passenger in the airline system
// ============================================================

class Passenger {
private:
    std::string passengerID;   // Unique ID (e.g., national ID or passport)
    std::string name;
    std::string email;
    std::string phone;

public:
    // Constructors
    Passenger() = default;
    Passenger(const std::string& id, const std::string& name,
        const std::string& email, const std::string& phone);

    // Getters
    std::string getPassengerID() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;

    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);

    // Display
    void display() const;

    // Serialization (for file I/O)
    std::string serialize() const;
    static Passenger deserialize(const std::string& line);
};

#endif // PASSENGER_H
