#include "user.hpp"

#include <string>

#include "exceptions.hpp"

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

void User::follow(User* user) {
    if (user == this)
        throw exceptions::BadRequest();

    auto it = std::find(followings_.begin(), followings_.end(), user);
    if (it != followings_.end())
        throw exceptions::BadRequest();

    followings_.push_back(user);
    user->addFollower(this);
}

void User::unfollow(User* user) {
    auto it = std::find(followings_.begin(), followings_.end(), user);
    if (it == followings_.end())
        throw exceptions::BadRequest();

    followings_.erase(it);
    user->removeFollower(this);
}

void User::addFollower(User* user) {
    auto it = std::find(followers_.begin(), followers_.end(), user);
    if (it != followers_.end())
        throw exceptions::BadRequest();

    followers_.push_back(user);
}

void User::removeFollower(User* user) {
    auto it = std::find(followers_.begin(), followers_.end(), user);
    if (it == followers_.end())
        throw exceptions::BadRequest();

    followers_.erase(it);
}