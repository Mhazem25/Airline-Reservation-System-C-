#pragma once
#ifndef MENU_H
#define MENU_H

#include <string>
#include "AirlineSystem.h"

using namespace std;

// ============================================================
// Menu.h - Console UI helpers and menu functions
// ============================================================

namespace Menu {

    // Utility I/O helpers
    void   printBanner();
    void   printLine(char c = '-', int width = 50);
    void   pause();
    string getInput(const string& prompt);
    int    getIntInput(const string& prompt, int minVal, int maxVal);
    double getDoubleInput(const string& prompt);

    // Input validation helpers
    bool isValidDate(const string& date);      // YYYY-MM-DD
    bool isValidTime(const string& time);      // HH:MM
    bool isValidEmail(const string& email);
    bool isValidPhone(const string& phone);

    // Top-level menus
    void loginMenu(AirlineSystem& sys);
    void adminMainMenu(AirlineSystem& sys);
    void userMainMenu(AirlineSystem& sys);

    // Admin sub-menus
    void manageFlightsMenu(AirlineSystem& sys);
    void manageReservationsMenu(AirlineSystem& sys);
    void manageUsersMenu(AirlineSystem& sys);

    // User sub-menus
    void searchFlightsMenu(AirlineSystem& sys);
    void bookTicketMenu(AirlineSystem& sys);
    void myReservationsMenu(AirlineSystem& sys);
}

#endif // MENU_H