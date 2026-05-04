# SkyLink Airline Reservation System ✈️

A console-based **airline reservation system** written in C++.  
It lets admins manage flights and users, and passengers search flights, book tickets, and manage their reservations.[web:88][web:91]

---

## Features

- **User authentication**
  - Login with username and password
  - Admin and regular user roles
- **Admin functions**
  - Add, remove, and list flights
  - View all reservations
  - Create new users (admin or user), optionally linked to a passenger ID
- **Passenger functions**
  - Search flights by destination, date, or route (origin + destination)
  - View all available flights
  - Book a ticket with seat selection and class (Economy, Business, First)
  - View reservations by passenger ID
  - Cancel an existing reservation
- **Seat management**
  - Per-flight seat map with available/booked seats
  - Prevents double-booking of seats
- **Persistent storage**
  - Data saved to simple `.dat` text files:
    - `data/flights.dat`
    - `data/passengers.dat`
    - `data/reservations.dat`
    - `data/users.dat`
- **Sample default data**
  - Automatically seeds:
    - Admin account: `admin / admin123`
    - User account: `john_doe / pass123`
    - Example flights and one sample passenger

---

## Project Structure

```text
.
├── data/
│   ├── flights.dat
│   ├── passengers.dat
│   ├── reservations.dat
│   └── users.dat
├── src/
│   ├── main.cpp
│   ├── AirlineSystem.cpp
│   ├── AirlineSystem.h
│   ├── Flight.cpp
│   ├── Flight.h
│   ├── Passenger.cpp
│   ├── Passenger.h
│   ├── Reservation.cpp
│   ├── Reservation.h
│   ├── User.cpp
│   ├── User.h
│   ├── Menu.cpp
│   └── Menu.h
└── README.md
```

Adjust paths if your layout is slightly different.

---

## Requirements

- C++ compiler with C++11 support or later (e.g. g++, clang++, MSVC)[web:86]
- Console/terminal environment
- On Windows, project can be compiled via Visual Studio or `g++` (MinGW). On Linux/macOS, `g++` or `clang++` works.

---

## Building and Running

From the project root (where your `src` folder is), you can compile with a simple command:

```bash
g++ src/*.cpp -o airline_reservation
```

Then run:

```bash
./airline_reservation
```

On Windows (MinGW):

```bash
g++ src\\*.cpp -o airline_reservation.exe
airline_reservation.exe
```

Make sure the `data/` directory exists in the same folder as the executable.  
If the data files are missing on first run, the system automatically creates default users, flights, and a passenger.

---

## Usage Overview

### Login

When you start the program, you see a login screen:

- Enter username and password.
- Example credentials:
  - Admin: `admin` / `admin123`
  - User: `john_doe` / `pass123`

### Admin Menu

As admin you can:

- **Manage Flights**
  - List all flights
  - Add new flights (flight number, origin, destination, date, departure/arrival time, seats, base price)
  - Remove flights
  - View seat map for a flight
- **Manage Reservations**
  - List all reservations
  - View a specific reservation by ID
  - Cancel a reservation
- **Manage Users**
  - Create new users (admin or user)
  - Optionally link a user to an existing passenger ID

### Passenger (User) Menu

As a regular user you can:

- **Search Flights**
  - By destination
  - By date
  - By route (origin + destination)
  - Or list all flights
- **Book a Ticket**
  - Choose a flight by flight number
  - Check available seats and seat map
  - Select seat and class (Economy, Business, First)
  - Enter passenger details (ID, name, email, phone)
  - Confirm booking and receive a reservation ID
- **My Reservations**
  - Enter your passenger ID to list all your reservations
- **Cancel a Reservation**
  - Enter a reservation ID to cancel it (also frees the seat)

---

## Data Storage Format (Brief)

Data is stored in simple pipe-delimited text lines:

- `Flight`:  
  `flightNumber|origin|destination|date|depTime|arrTime|totalSeats|basePrice|seatBitmask`
- `Reservation`:  
  `resID|active|flightNum|seatNum|class|price|bookDate|passengerSerialized`
- `Passenger`:  
  `ID|name|email|phone`
- `User`:  
  `username|passwordHash|role|linkedPassengerID`

This makes it easy to inspect and edit files manually if needed.

---

## Limitations & Notes

- Password hashing is a very **simple demo hash**, not secure for real applications.
- No concurrency or multi-user access handling.
- No GUI; it is purely a console application aimed at learning OOP, file I/O, and basic system design in C++.[web:88][web:91]

---

## Possible Improvements

- Replace demo password hashing with a real cryptographic hash.
- Add input validation for dates/times beyond simple format checks.
- Add unit tests.
- Add export/reporting features (e.g., daily flights, revenue).
- Port to a GUI framework or web backend in the future.

---

## License

Choose a license that fits your needs, for example:

```text
MIT License
```

You can add a `LICENSE` file in the repository and reference it here.[web:87]
