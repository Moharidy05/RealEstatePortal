#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector> // Included because vector was in the original RealEstatePortal code, assuming User might be used in a vector
#include<unordered_map>
#include <fstream> // Included for completeness if file operations related to User are added later
#include "Property.h" // Ensure Property.h defines the Property class

using namespace std; // Included as requested

class User {
public:
    int userId;
    string username;
    string password;
    string email;
    string phone;

    bool active = false; // Default member initializer

    // Constructor declaration
    User(int id, string uname, string pwd, string mail, string ph = "");

    // Member function declarations

    void viewProfile();
    void editProfile();
    void changePassword();
   
 /* void displayProperties(unordered_map <int,Property> &properties); */
    // If you implement load/save for User, add declarations here:
    // void loadFromFile(istream& is);
    // void saveToFile(ostream& os) const;
};

#endif // USER_H