#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "User.h"
#include "Property.h"

using namespace std;

class Admin : public User {
public:
    Admin(int id, string uname, string pwd, string mail, string ph = "");

    static Admin* login(vector<Admin>& admins);
    void showDashboard();
    void listUsers(const vector<User>& users);
    void approveListing(unordered_map<int, Property>& properties);
    void addUser(vector<User>& users);
    void removeUser(vector<User>& users);
    void disableUser(vector<User>& users);
    void manageProfile();
    void manageUser(vector<User>& users);
    void handleActions(vector<User>& users, unordered_map<int, Property>& properties, const vector<Admin>& admins);
    void activateUser(vector<User>& users);
    void generateReport(const vector<User>& users, const unordered_map<int, Property>& properties); // New function

    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};
#endif