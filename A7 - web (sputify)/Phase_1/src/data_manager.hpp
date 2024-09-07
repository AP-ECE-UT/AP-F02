#ifndef DATA_MANAGER_HPP_INCLUDE
#define DATA_MANAGER_HPP_INCLUDE

#include <vector>

#include "music.hpp"
#include "playlist.hpp"
#include "user.hpp"

class DataManager {
public:
    DataManager();
    User* findUserByName(const std::string& username) const;
    User* findUserByID(int id) const;
    Music* findMusicByID(int id) const;
    std::vector<Music*> getMusics(const std::string& name = "",
                                  const std::string& artist = "",
                                  const std::string& tag = "") const;
    std::vector<User*> getAllUsers() const;

    bool isUsernameTaken(const std::string& username) const;
    void addUser(User* user);
    void addMusic(Music* music);
    void removeMusic(Music* music);

private:
    int available_user_id_;
    int available_music_id_;
    std::vector<Music*> musics_;
    std::vector<User*> users_;
};

#endif // DATA_MANAGER_HPP_INCLUDE
