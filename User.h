#ifndef USER_H
#define USER_H

#include <string>

// ============================================================
// User.h - Represents a system user (admin or passenger)
// ============================================================

using namespace std;

enum class UserRole { ADMIN, USER };

class User {
private:
    string  username;
    string  passwordHash;      // Simple hash (demo only)
    UserRole role;
    string  linkedPassengerID; // For USER role

public:
    User() = default;
    User(const string& username,
        const string& password,
        UserRole role,
        const string& linkedPassengerID = "");

    string   getUsername()          const;
    UserRole getRole()              const;
    string   getLinkedPassengerID() const;
    bool     checkPassword(const string& password) const;
    bool     isAdmin()              const;

    // Serialization
    string        serialize() const;
    static User   deserialize(const string& line);

private:
    // Very simple deterministic hash (for demo only — not cryptographic)
    static string hashPassword(const string& password);
};

#endif // USER_H