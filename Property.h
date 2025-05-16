#ifndef PROPERTY_H
#define PROPERTY_H

#include <iostream>
#include <unordered_map> // Included for the unordered_map used in PropertyManagement
#include <string>
#include "User.h"

using namespace std; // Included as requested

// Declaration of the Property struct
struct Property {
    int id;
    int userId;
    string location;
    float price;
    string description;
    int bedrooms;
    string type;
    double area;
    bool isHighlighted = false; // Default member initializer
    bool approved = false;      // Default member initializer
    
    // Member function declaration
    void display();
};

// Declaration of the PropertyManagement class
class PropertyManagement {
public:
    // Member function declarations
    void addProperty(int userId,unordered_map<int, Property>& properties);
    void removeProperty(unordered_map<int, Property>& properties);
    void editProperty(unordered_map<int, Property>& properties);
    void highlightProperty(unordered_map<int, Property>& properties);
    void displayAllProperties(unordered_map<int, Property>& properties);
    void manageProperties(int userId,unordered_map<int, Property>& properties);
    void removeUserProperty(int userId,unordered_map<int, Property>& properties);
};

#endif // PROPERTY_H