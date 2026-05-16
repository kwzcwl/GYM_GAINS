#include "Database.h"
#include <fstream>
#include <iostream>

Database::Database(std::string dbFilename) : filename(dbFilename) {}

Database::~Database()
{
    for (User* u : users)
    {
        delete u; 
    }
    users.clear();
}

/*void Database::load()
{
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return;

    size_t userSize;
    if (!(ifs >> userSize)) return;
    ifs.ignore();

    for (size_t i = 0; i < userSize; ++i)
    {
        User* loadedUser = User::load(ifs);
        if (loadedUser != nullptr)
        {
            users.push_back(loadedUser);
        }
    }
    ifs.close();
}
*/
void Database::save()
{
    std::ofstream ofs(filename);
    if (!ofs.is_open()) return;

    ofs << users.size() << "\n";
    for (User* u : users)
    {
        u->save(ofs);
    }
    ofs.close();
}

User* Database::authenticate(const std::string& username, const std::string& password) {
    for (User* u : users)
    {
        if (u->username == username && u->checkPassword(password)) {
            return u;
        }
    }
    return nullptr;
}

void Database::addUser(User* newUser)
{
    users.push_back(newUser);
}