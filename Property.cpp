#include "Property.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <limits> // Required for numeric_limits
#include"User.h"
using namespace std; // Included as requested

// Definition of the Property struct's display function

void Property::display() {
    cout << "ID: " << id << "\nLocation: " << location << "\nPrice: " << price
         << "\nDescription: " << description << "\nBedrooms: " << bedrooms
         << "\nHighlighted: " << (isHighlighted ? "Yes" : "No")
         << "\nApproved: " << (approved ? "Yes" : "No") << "\n"; // Added approved status
}

// Definitions of the PropertyManagement class member functions

void PropertyManagement::addProperty(int userId,unordered_map<int, Property>& properties) {
    Property p;
    p.userId=userId;
    cout << "Enter Property ID: ";
    cin >> p.id;
    // Check if ID already exists
    if (properties.count(p.id)) {
        cout << "Error: Property with ID " << p.id << " already exists.\n";
        // Clear the rest of the line after reading the ID
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter Location: ";
    // Clear the newline character left by cin >> p.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, p.location);

    cout << "Enter Price: ";
    cin >> p.price;
    // Clear the newline character left by cin >> p.price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Description: ";
    getline(cin, p.description);
    cout<<" Enter the type : ";
    getline(cin,p.type);
    cout<<"Enter the area of the property : ";
    cin>>p.area;
    cout << "Enter Number of Bedrooms: ";
    cin >> p.bedrooms;
    // Clear the newline character left by cin >> p.bedrooms;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    // Properties are initially not approved or highlighted when added
    p.isHighlighted = false;
    p.approved = false;

    properties[p.id] = p;
    cout << "Property added successfully. Waiting for admin approval.\n";
}

void PropertyManagement::removeProperty(unordered_map<int, Property>& properties) {
    int id;
    cout << "Enter Property ID to remove: ";
    cin >> id;
    // Clear the newline character
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (properties.erase(id)) {
        cout << "Property removed successfully.\n";
    } else {
        cout << "Property not found.\n";
    }
}

void PropertyManagement::removeUserProperty(int userId,unordered_map<int, Property>& properties){
    vector<Property>prop;
    for(auto& pair : properties){
        if(pair.second.userId==userId){
            prop.push_back(pair.second);
        }
    }
    for(Property p : prop){
        p.display();
        cout<<"---------------------\n";
    }
    int id;
    cout<<"Enter the id of the property : ";
    cin>>id;
    for(Property p : prop){
        if(id==p.id && properties.erase(id)){
             cout << "Property removed successfully.\n";
             return;
        }
    }
    cout<<"Invalid property id\n";
}

void PropertyManagement::editProperty(unordered_map<int, Property>& properties) {
    int id;
    cout << "Enter Property ID to edit: ";
    cin >> id;
    // Clear the newline character
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto it = properties.find(id);
    if (it != properties.end()) {
        Property& p = it->second;
        cout << "Enter new price: ";
        cin >> p.price;
        // Clear the newline character
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter new description: ";
        getline(cin, p.description);

        cout << "Edit successful.\n";
    } else {
        cout << "Property not found.\n";
    }
}

void PropertyManagement::highlightProperty(unordered_map<int, Property>& properties) {
    int id;
    cout << "Enter Property ID to highlight: ";
    cin >> id;
    // Clear the newline character
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto it = properties.find(id);
    if (it != properties.end()) {
        // Only highlight if approved
        if (it->second.approved) {
             it->second.isHighlighted = true;
             cout << "Property highlighted.\n";
        } else {
            cout << "Property is not yet approved and cannot be highlighted.\n";
        }
    } else {
        cout << "Property not found.\n";
    }
}

void PropertyManagement::displayAllProperties(unordered_map<int, Property>& properties) {
    if (properties.empty()) {
        cout << "No properties available.\n";
        return;
    }
    cout << "\n--- All Properties ---\n";
    for (auto& pair : properties) {
        pair.second.display();
        cout << "------------------------\n";
    }
}

void PropertyManagement::manageProperties(int userId,unordered_map<int, Property>& properties) {
    int choice;
    while (true) {
        cout << "\n--- Manage Properties Menu ---\n";
        cout << "1. Add Property\n2. Remove Property\n3. Edit Property\n4. Highlight Property\n5. Display All Properties\n0. Exit\nChoose: ";
        cin >> choice;

        if (cin.fail()) { // Handle non-integer input
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
            continue; // Go back to the start of the loop
        }

        if (choice == 0) {
            break; // Exit the loop
        }

        switch (choice) {
            case 1: addProperty(userId,properties); break;
            case 2: removeProperty(properties); break;
            case 3: editProperty(properties); break;
            case 4: highlightProperty(properties); break;
            case 5: displayAllProperties(properties); break;
            default: cout << "Invalid choice.\n"; break;
        }
         // Add a small delay or prompt to continue to prevent menu from flashing
        // cout << "Press Enter to continue...";
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // cin.get(); // Wait for user to press Enter
    }
}