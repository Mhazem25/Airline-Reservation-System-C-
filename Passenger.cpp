// ============================================================
// Passenger.cpp - Implementation of the Passenger class
// ============================================================

#include "Passenger.h"
#include <iostream>
#include <sstream>

using namespace std;

Passenger::Passenger(const string& id, const string& name,
    const string& email, const string& phone)
    : passengerID(id), name(name), email(email), phone(phone) {
}

string Passenger::getPassengerID() const { return passengerID; }
string Passenger::getName()        const { return name; }
string Passenger::getEmail()       const { return email; }
string Passenger::getPhone()       const { return phone; }

void Passenger::setName(const string& n) { name = n; }
void Passenger::setEmail(const string& e) { email = e; }
void Passenger::setPhone(const string& p) { phone = p; }

void Passenger::display() const {
    cout << "  Passenger ID : " << passengerID << "\n"
        << "  Name         : " << name << "\n"
        << "  Email        : " << email << "\n"
        << "  Phone        : " << phone << "\n";
}

// Serialize to a pipe-delimited string: ID|name|email|phone
string Passenger::serialize() const {
    return passengerID + "|" + name + "|" + email + "|" + phone;
}

// Deserialize from a pipe-delimited string
Passenger Passenger::deserialize(const string& line) {
    istringstream ss(line);
    string id, nm, em, ph;
    getline(ss, id, '|');
    getline(ss, nm, '|');
    getline(ss, em, '|');
    getline(ss, ph, '|');
    return Passenger(id, nm, em, ph);
}