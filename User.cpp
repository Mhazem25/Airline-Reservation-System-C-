// ============================================================
// User.cpp - Implementation of the User class
// ============================================================

#include "User.h"
#include <sstream>
#include <numeric>
using namespace std;

User::User(const string& username,
    const string& password,
    UserRole role,
    const string& linkedPassengerID)
    : username(username),
    passwordHash(hashPassword(password)),
    role(role),
    linkedPassengerID(linkedPassengerID) {
}

string   User::getUsername()          const { return username; }
UserRole User::getRole()              const { return role; }
string   User::getLinkedPassengerID() const { return linkedPassengerID; }
bool     User::isAdmin()              const { return role == UserRole::ADMIN; }

bool User::checkPassword(const string& password) const {
    return hashPassword(password) == passwordHash;
}

// Very simple demo hash: sum of (char * position) mod 99991, encoded as string
string User::hashPassword(const string& password) {
    unsigned long long h = 5381;
    for (size_t i = 0; i < password.size(); ++i)
        h = ((h << 5) + h) + (unsigned char)password[i] + i * 31;
    return to_string(h % 99999999ULL);
}

string User::serialize() const {
    return username + "|" + passwordHash + "|"
        + (role == UserRole::ADMIN ? "ADMIN" : "USER")
        + "|" + linkedPassengerID;
}

User User::deserialize(const string& line) {
    istringstream ss(line);
    string uname, phash, roleStr, linked;
    getline(ss, uname, '|');
    getline(ss, phash, '|');
    getline(ss, roleStr, '|');
    getline(ss, linked);

    UserRole r = (roleStr == "ADMIN") ? UserRole::ADMIN : UserRole::USER;
    User u;
    u.username = uname;
    u.passwordHash = phash;
    u.role = r;
    u.linkedPassengerID = linked;
    return u;
}