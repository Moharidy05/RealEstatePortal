#include "User.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits> // Required for numeric_limits
#include<unordered_map>
#include"Property.h"
#include"Admin.h"
using namespace std; // Included as requested

// Constructor definition
User::User(int id, string uname, string pwd, string mail, string ph)
    : userId(id), username(uname), password(pwd), email(mail), phone(ph)
{
    // Constructor body if needed for additional initialization
}

// Member function definitions

void User::viewProfile() {
    cout << "-----------------------------------" << endl;
    cout << "\nProfile\nName: " << username
         << "\nEmail: " << email
         << "\nPhone: " << phone;

    cout << "\n-----------------------------------" << endl;
}

void User::editProfile() {
    cout << "-----------------------------------" << endl;
    cout << "\nEdit Profile\nCurrent name: " << username << "\nNew name: ";
    // Consume the newline character left in the buffer by previous cin operations
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);

    cout << "Current phone: " << phone << "\nNew phone: ";
    getline(cin, phone); // Use getline for phone as well in case of spaces

    cout << "Profile updated!\n";
    cout << "-----------------------------------" << endl;
}

void User::changePassword() {
    cout << "-----------------------------------" << endl;
    string oldPass;
    cout << "Enter the old password: ";
    // Consume newline if necessary before reading password
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, oldPass);

    if (oldPass == password) { // Simplified password verification
        cout << "Enter the new password: ";
        getline(cin, password);
        cout << "The password has changed successfully!\n";
    } else {
        cout << "The password entered is wrong.\n";
    }
    cout << "-----------------------------------" << endl;
}

/* void User::displayProperties(unordered_map<int,Property>&properties){
    for(auto& pair : properties){
        if(pair.second.userId==this->userId){
            pair.second.display();
            cout<<"----------------------------------\n";
        }
    }
} */

// If you implement load/save for User, define them here:
/*
void User::loadFromFile(istream& is) {
    // Implement loading user data from the input stream
    // Make sure to handle different data types and delimiters
}

void User::saveToFile(ostream& os) const {
    // Implement saving user data to the output stream
    // Make sure to handle different data types and delimiters
}
*/