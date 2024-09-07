#include "data_manager.hpp"

#include <algorithm>

#include "consts.hpp"
#include "exceptions.hpp"

DataManager::DataManager() : available_user_id_(consts::USERS_START_ID), available_music_id_(consts::MUSICS_START_ID) {}

User* DataManager::findUserByName(const std::string& username) const {
    auto it = std::find_if(users_.begin(), users_.end(), [&](const User* user) {
        return user->getName() == username;
    });

    if (it != users_.end())
        return *it;

    throw exceptions::NotFound();
}

User* DataManager::findUserByID(int id) const {
    auto it = std::find_if(users_.begin(), users_.end(), [&](const User* user) {
        return user->getID() == id;
    });

    if (it != users_.end())
        return *it;

    throw exceptions::NotFound();
}

Music* DataManager::findMusicByID(int id) const {
    auto it = std::find_if(musics_.begin(), musics_.end(), [&](const Music* music) {
        return music->getID() == id;
    });

    if (it != musics_.end())
        return *it;

    throw exceptions::NotFound();
}

std::vector<Music*> DataManager::getMusics(const std::string& name,
                                           const std::string& artist,
                                           const std::string& tag) const {
    std::vector<Music*> result;
    std::copy_if(musics_.begin(), musics_.end(), std::back_inserter(result),
                 [&](const Music* music) {
                     return music->matchesFilters(name, artist, tag);
                 });
    return result;
}

std::vector<User*> DataManager::getAllUsers() const {
    return users_;
}

bool DataManager::isUsernameTaken(const std::string& username) const {
    return std::any_of(users_.begin(), users_.end(), [&](const User* user) {
        return user->getName() == username;
    });
}

void DataManager::addUser(User* user) {
    user->setID(available_user_id_++);
    users_.push_back(user);
}

void DataManager::addMusic(Music* music) {
    music->setID(available_music_id_++);
    musics_.push_back(music);
}

void DataManager::removeMusic(Music* music) {
    for (auto user : users_)
        user->removeMusic(music);

    musics_.erase(std::remove(musics_.begin(), musics_.end(), music), musics_.end());
}