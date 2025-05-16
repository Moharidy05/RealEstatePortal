#include "RealEstatePortal.h"
#include "User.h" // Assuming you have a user.h header file
// #include "admin.h" // Uncomment if you have an admin.h header file
#include"Admin.h"
#include"Property.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits> // Required for numeric_limits
#include <iomanip> // Required for setw

using namespace std; // Included as requested

// Helper function to find a user by ID
User* RealEstatePortal::findUserById(int userId) {
    for (auto& user : users) {
        if (user.userId == userId) {
            return &user;
        }
    }
    return nullptr; // User not found
}


// Constructor: Loads data from files
RealEstatePortal::RealEstatePortal() {
    cout<<"load data is called\n";
    loadData();
}

// Destructor: Saves data to files
RealEstatePortal::~RealEstatePortal() {
    saveData();
}

// --- Data Persistence Functions ---

void RealEstatePortal::loadData() {
    //load admins
    ifstream adminFile(ADMINS_FILE);
    if (adminFile.is_open()) {
        string line;
        while (getline(adminFile, line)) {
            stringstream ss(line);
            string segment;
            vector<string> segments;
            while (getline(ss, segment, '|')) {
                segments.push_back(segment);
            }
            if (segments.size() == 5) {
                try { // Added try-catch for admin parsing
                    int id = stoi(segments[0]);
                    admins.emplace_back(id, segments[1], segments[2], segments[3], segments[4]);
                } catch (const std::invalid_argument& e) {
                    cout << "Error parsing admin line (invalid argument): '" << line << "'. Details: " << e.what() << endl;
                } catch (const std::out_of_range& e) {
                     cout << "Error parsing admin line (out of range): '" << line << "'. Details: " << e.what() << endl;
                }
            } else {
                 cout << "Warning: Skipping malformed admin line: '" << line << "' (Expected 5 segments, found " << segments.size() << ")" << endl;
            }
        }
        adminFile.close();
    } else {
        cout << "Warning: Could not open " << ADMINS_FILE << " for reading. Starting with no admins.\n";
    }

    // Load Users 
    ifstream userFile(USERS_FILE);
    if (userFile.is_open()) {
        string line;
        int maxUserId = 0;
        while (getline(userFile, line)) {
            stringstream ss(line);
            string segment;
            vector<string> segments;
            while (getline(ss, segment, '|')) {
                segments.push_back(segment);
            }
            if (segments.size() == 6) {
                try { // Added try-catch for user parsing
                    int id = stoi(segments[0]);
                    bool active = (segments[5] == "1"); // This might also need a try-catch if segments[5] isn't "0" or "1"
                    users.emplace_back(id, segments[1], segments[2], segments[3], segments[4]);
                    users.back().active = active; // Set the active status
                    if (id > maxUserId) {
                        maxUserId = id;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Error parsing user line (invalid argument): '" << line << "'. Details: " << e.what() << endl;
                } catch (const std::out_of_range& e) {
                     cout << "Error parsing user line (out of range): '" << line << "'. Details: " << e.what() << endl;
                }
            } else {
                 cout << "Warning: Skipping malformed user line: '" << line << "' (Expected 6 segments, found " << segments.size() << ")" << endl;
            }
        }
        userFile.close();
        nextUserId = maxUserId + 1;
    } else {
        cout << "Warning: Could not open " << USERS_FILE << " for reading. Starting with no users.\n";
        nextUserId = 1;
    }

    // Load Properties
    ifstream propertyFile(PROPERTIES_FILE);
    if (propertyFile.is_open()) {
        string line;
        while (getline(propertyFile, line)) {

            stringstream ss(line);
            string segment;
            vector<string> segments;
            while (getline(ss, segment, '|')) {
                segments.push_back(segment);
            }

            if (segments.size() == 10) {
                try { // Added try-catch for property parsing
                    int id = stoi(segments[0]);
                    int ownerId = stoi(segments[1]);
                    string location = segments[2];
                    float price = stof(segments[3]);
                    string description = segments[4];
                    int bedrooms = stoi(segments[5]);
                    string type = segments[6];
                    double area = stod(segments[7]);
                    bool highlighted = (segments[8] == "1");
                    bool approved = (segments[9] == "1");

                    properties[id] = {id, ownerId, location, price, description, bedrooms, type, area, highlighted, approved};

                } catch (const std::invalid_argument& e) {
                    cout << "Error parsing property line (invalid argument): '" << line << "'. Details: " << e.what() << endl;
                } catch (const std::out_of_range& e) {
                     cout << "Error parsing property line (out of range): '" << line << "'. Details: " << e.what() << endl;
                }
            } else {
                 cout << "Warning: Skipping malformed property line: '" << line << "' (Expected 10 segments, found " << segments.size() << ")" << endl;
            }
        }
        propertyFile.close();
    } else {
        cout << "Warning: Could not open " << PROPERTIES_FILE << " for reading. Starting with no properties.\n";
    }
}

void RealEstatePortal::saveData() {
    // Save Admins
    ofstream adminFile(ADMINS_FILE);
    if (adminFile.is_open()) {
        for (const auto& admin : admins) {
            adminFile << admin.userId << "|" << admin.username << "|" << admin.password << "|" << admin.email << "|" << admin.phone << "\n";
        }
        adminFile.close();
    } else {
        cout << "Error: Could not open " << ADMINS_FILE << " for writing.\n";
    }

    // Save Users
    ofstream userFile(USERS_FILE);
    if (userFile.is_open()) {
        for (const auto& user : users) {
            userFile << user.userId << "|" << user.username << "|" << user.password << "|" << user.email << "|" << user.phone << "|" << (user.active ? "1" : "0") << "\n";
        }
        userFile.close();
    } else {
        cout << "Error: Could not open " << USERS_FILE << " for writing.\n";
    }

    // Save Properties (Updated to match the struct member order: id|userId|location|...)
    ofstream propertyFile(PROPERTIES_FILE);
    if (propertyFile.is_open()) {
        for (const auto& pair : properties) {
            const auto& p = pair.second;
            // Write in the order: id|userId|location|price|description|bedrooms|type|area|isHighlighted|approved
            propertyFile << p.id << "|"
                         << p.userId << "|" // <-- Write userId here
                         << p.location << "|"
                         << p.price << "|"
                         << p.description << "|"
                         << p.bedrooms << "|"
                         << p.type << "|"
                         << p.area << "|"
                         << (p.isHighlighted ? "1" : "0") << "|"
                         << (p.approved ? "1" : "0") << "\n";
        }
        propertyFile.close();
    } else {
        cout << "Error: Could not open " << PROPERTIES_FILE << " for writing.\n";
    }
}


bool RealEstatePortal::registerUser() {
    system("cls"); // Clear the console (Windows specific)
    string username, password, email, phone;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter phone: ";
    getline(cin, phone);
    

    for (const auto& user : users) {
        if (user.phone == phone || user.username == username) {
            cout << "Username or phone number already exists!\n";
            return false;
        }
    }
    users.emplace_back(nextUserId++, username, password, email, phone); // Assuming User constructor matches this signature

    cout << "Registration successful!\n";
    return true;
}

User* RealEstatePortal::login(string username, string password) {
    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!\n";
            return &user;
        }
    }
    cout << "Invalid username or password!\n";
    return nullptr;
}
void RealEstatePortal::compareProperties() {
    PropertyManagement propertyManager;
    propertyManager.displayAllProperties(properties);
    vector<int> propertyIds;
    cout << "You can only compare up to 4 properties at once.\n";
    cout << "Enter the IDs of the properties you want to compare (enter -1 to stop): \n";

    for (int i = 0; i < 4; i++) {
        int id;
        cout << "Property " << (i + 1) << ": ";
        cin >> id;

        if (id == -1) {
            break; // Exit the loop if the user enters -1
        }

        propertyIds.push_back(id);
    }
    
    

    cout << "Comparing Properties:\n";
    cout << left << setw(15) << "Field";

    // Print property headers
    for (int id : propertyIds) {
        if (properties.find(id) != properties.end()) {
            cout << setw(25) << ("Property ID " + to_string(id));
        }
        else {
            cout << setw(25) << ("[ID " + to_string(id) + " Not Found]");
        }
    }
    cout << endl;

    // Define the fields to compare
    vector<string> fields = { "Location", "Price", "Bedrooms", "Type", "Square Ft" };

    // Print each field for all properties
    for (const string& field : fields) {
        cout << left << setw(15) << field;
        for (int id : propertyIds) {
            auto it = properties.find(id);
            if (it != properties.end()) {
                const Property& p = it->second;
                if (field == "Location") cout << setw(25) << p.location;
                else if (field == "Price") cout << setw(25) << p.price;
                else if (field == "Bedrooms") cout << setw(25) << p.bedrooms;
                else if (field == "Type") cout << setw(25) << p.type;
                else if (field == "Square Ft") cout << setw(25)<<p.area;
            }
            else {
                cout << setw(25) << "N/A";
            }
        }
        cout << endl;
    }
}
void RealEstatePortal:: searchProperties(){
    string locationFilter; // Removed std::
    double minPrice ;                   // Minimum price of $300,000
    double maxPrice;                   // Maximum price of $600,000
    int minBedrooms ;                          // Minimum of 2 bedrooms
    string typeFilter ;         // Removed std:: // Search specifically for apartments
    float minarea;             // Minimum square footage of 1000
    float maxarea;             // Maximum square footage of 2000

     cout << "Enter location filter (leave empty for no filter): ";
    getline(cin, locationFilter);

    cout << "Enter minimum price (e.g., 300000): ";
    cin >> minPrice;

    cout << "Enter maximum price (e.g., 600000): ";
    cin >> maxPrice;

    cout << "Enter minimum number of bedrooms (e.g., 2): ";
    cin >> minBedrooms;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
    cout << "Enter property type filter (leave empty for no filter): ";
    getline(cin, typeFilter);

    cout << "Enter minimum square footage (e.g., 1000): ";
    cin >> minarea;

    cout << "Enter maximum square footage (e.g., 2000): ";
    cin >> maxarea;

    
    vector<Property> results;

    for (const auto& pair : properties) {
        const Property& p = pair.second;

        bool locationMatch = locationFilter.empty() || p.location == locationFilter;
        bool priceMatch = p.price >= minPrice && p.price <= maxPrice;
        bool bedroomsMatch = p.bedrooms >= minBedrooms;
        bool typeMatch = typeFilter.empty() || p.type == typeFilter;
        bool areaMatch = p.area >= minarea && p.area <= maxarea;

        if (locationMatch && priceMatch && bedroomsMatch && typeMatch && areaMatch) {
            results.push_back(p);
        }
    }

    cout << "--- Search Results ---" << endl; // Removed std::
    if (results.empty()) {
        cout << "No properties found matching the criteria." << endl; // Removed std::
    } else {
        for (auto& property : results) {
            property.display(); // Assuming Property has a display() method
            cout << "--------------------" << endl; // Removed std::
        }
    }

    
}
void RealEstatePortal::manageUserProfile(User* loggedInUser) {
    system("cls"); // Clear the console (Windows specific)
    int choice;
    while (true) {
        cout << "\n=== User Dashboard ===\n";
        cout << "1. View Profile\n2. Edit Profile\n3. Change password\n 4. Properties  listed by you \n 5.remove property listed\n 6.go back\nChoose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer

        if (choice == 6) { // Logout
            break;
        }

        bool flag=false;
        switch (choice) {
            case 1:
                loggedInUser->viewProfile(); // You need to implement User::viewProfile()
                break;
            case 2:
                loggedInUser->editProfile(); // You need to implement User::editProfile()
                break;
            case 3:
                loggedInUser->changePassword(); // You need to implement User::changePassword()
                break;
            case 4:
                for(auto& pair : properties){
                    if(pair.second.userId==loggedInUser->userId){
                        pair.second.display();
                        flag=true;
                        cout<<"----------------------------------\n";
                    }
                }
                if(!flag){
                    cout<<"No properties listed \n";
                }
                break;
            case 5:
                PropertyManagement propertyManager;
                propertyManager.removeUserProperty(loggedInUser->userId,properties);
                break;
            default:
                cout << "Invalid option!\n";
                break;
        }
    }
}

void RealEstatePortal::buyProperty(User* user){
    int choice;

    while(true){
        vector<Property>canbeBought;
        for (auto& pair : properties) {
            if(pair.second.userId!=user->userId){
                canbeBought.push_back(pair.second);
            }
        }
        if (canbeBought.empty()) {
            cout << "No properties available.\n";
            return;
        }
        cout << "\n--- Properties you can buy ---\n";
        for(Property property : canbeBought){
            property.display();
            cout<<"------------------------\n";
        }

        cout<<"Press 0 to go back : ";
        cin>>choice;
        if(choice==0) return;

    }
}
void RealEstatePortal::run()
{
    int choice;
    system("cls"); // Clear the console (Windows specific)

    while (true) {
        cout << "\n=== Real Estate Portal ===\n";
        cout << "You are : \n 1.Admin \n 2.User\n";
        cout<<"Press any key to exit\n";
        cin>>choice;
        if(choice==1)
        {
            Admin* admin=Admin::login(admins);

            if(admin != nullptr){
                admin->handleActions(users,properties,admins);
            }
            else{
                return;
            }
        }
        else if(choice==2){ 
            cout << "1. Register\n2. Login\n3. Exit\nChoose an option: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Use ignore with limits to clear the buffer

            if (choice == 3) {
                break;
            }

            switch (choice) {
                case 1: {
                    registerUser();
                    break;
                }

                case 2: {
                    string username, password;
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    User* loggedInUser = login(username, password);
                    if (loggedInUser) {
                        while (true) {
                            cout << "\n=== User Dashboard ===\n";
                            cout<<"1.Manage Profile\n2.buy a property\n3.list a property\n4.search aproperty\n5.compare between properties\n6.Logout"; 
                            cout << "\nChoose an option: ";
                            cin >> choice;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer

                            if (choice == 6) { // Logout
                                break;
                            }
                            switch(choice){
                                case 1:
                                    manageUserProfile(loggedInUser);
                                    break;
                                case 2:
                                    if(loggedInUser->active){
                                        buyProperty(loggedInUser);
                                    }
                                    else{
                                        cout<<"You are not active. Contact the admin activation\n";
                                    }
                                    break;
                                case 3:
                                    PropertyManagement propertyManager;
                                     if(loggedInUser->active){
                                        propertyManager.addProperty(loggedInUser->userId,properties);
                                
                                    }
                                    else{
                                        cout<<"You are not active. Contact the admin for activation\n";
                                    }
                                    break;
                                case 4:
                                     if(loggedInUser->active){
                                         searchProperties();
                                        
                                    }
                                    else{
                                        cout<<"You are not active. Contact the admin for activation\n";
                                    }
                                    break;
                                case 5:
                                    if(loggedInUser->active){
                                        compareProperties();
                                    }
                                    else{
                                        cout<<"You are not active. Contact the admin for activation\n";
                                    }
                                    break;
                                default:
                                    cout<<"Invalid Option\n";

                            }

                            
                        }
                    }
                    break;
                }

                default:
                    cout << "Invalid option!\n";
                    break;
            }
        }
        else{
            cout << "Invalid option!\n";
            return;
        }

    }
}

int main(){
    
        
        // --- Define the filter parameters you want to use ---
    RealEstatePortal *portal=new RealEstatePortal();
    portal->run();
    delete portal;
    // --- End of filter parameters ---
    // Your code here...
    // ...
    system("pause");
    return 0;

      
        

}