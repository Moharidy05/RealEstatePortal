#ifndef REALESTATEPORTAL_H
#define REALESTATEPORTAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>  

#include "User.h"
#include "Admin.h"
#include "Property.h"

using namespace std;

// Forward declarations if needed (not strictly necessary now)
class User;

class RealEstatePortal {
public:
 // Constructor and Destructor to handle loading and saving
    RealEstatePortal();
    ~RealEstatePortal();

    vector<User> users;
    int nextUserId = 1;
    vector<Admin> admins;
    unordered_map<int, Property> properties;
    // File names for persistence
    const string USERS_FILE = "users.txt";
    const string ADMINS_FILE = "admins.txt";
    const string PROPERTIES_FILE = "properties.txt";

    // Private helper functions for loading and saving
    void loadData();
    void saveData();

    // Helper function to find a user by ID (useful for adding properties)
    User* findUserById(int userId);
    
   

    bool registerUser();
    User* login(string username, string password);
    void compareProperties() ;
    void searchProperties();
    void buyProperty(User* user);
    void manageUserProfile(User*loggedInUser);
    void run();
    
};

#endif // REALESTATEPORTAL_H
