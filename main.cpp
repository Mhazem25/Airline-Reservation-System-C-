#include <iostream>
#include "AirlineSystem.h"
#include "Menu.h"
using namespace std;
int main()
{
    // 1. Initialize the system
    // This will trigger the constructors that load data from:
    // data/flights.dat, data/passengers.dat, data/reservations.dat, data/users.dat
    AirlineSystem skyLink;
    try 
    {
        // 2. Launch the Login Menu
        // This loop ensures that if a user logs out, the program 
        // returns to the login screen rather than exiting.
        while (true) 
        {
            Menu::loginMenu(skyLink);
        }
    }
    catch (const exception& e) 
    {
        // Basic error handling for critical system failures
        cerr << "\n[CRITICAL ERROR] A system error occurred: " << e.what() << endl;
        cout << "Press Enter to exit..." << endl;
        cin.get();
        return 1;
    }

    return 0;
}