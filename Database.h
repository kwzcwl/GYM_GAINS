#pragma once

#include <vector>
#include <string>
#include "User.h"

class Database {
private:
    std::vector<User*> users;
    std::string filename;

public:
    Database(std::string dbFilename);
    ~Database();

    //void load();
    //void save() const;
    
    User* authenticate(const std::string& username, const std::string& password);
    void addUser(User* newUser);
    
    const std::vector<User*>& getUsers() const { return users; }
};