#include "user.hpp"

#include <string>

User::User(const std::string& username, const std::string& password)
    : username_(username), password_(password) {}

bool User::validatePassword(const std::string& password) const {
    return password_ == password;
}

std::string User::getName() const {
    return username_;
}

int User::getID() const {
    return id_;
}

void User::setID(int id) {
    id_ = id;
}
