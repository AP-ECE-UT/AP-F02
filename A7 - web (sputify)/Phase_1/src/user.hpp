#ifndef USER_HPP_INCLUDE
#define USER_HPP_INCLUDE

#include <string>

#include "types.hpp"

class PlayList;
class Music;

class User {
public:
    User() = delete;
    User(const std::string& username, const std::string& password);

    bool validatePassword(const std::string& password) const;
    std::string getName() const;
    int getID() const;
    void setID(int id);

    virtual void createPlaylist(const std::string& name) = 0;
    virtual void addMusicToPlaylist(const std::string& name, Music* music) = 0;
    virtual void removeMusic(Music* music) = 0;
    virtual Music* createMusic(const std::string& title, const std::string& path, const std::string& year,
                               const std::string& album, const std::vector<std::string>& tags,
                               const hh_mm_ss& duration) = 0;
    virtual std::vector<PlayList*> getPlaylists() const = 0;
    virtual std::vector<Music*> getMusics() const = 0;

    virtual std::string shortInfoToString() const = 0;
    virtual std::string longInfoToString() const = 0;

protected:
    int id_;
    std::string username_;
    std::string password_;
};

#endif // USER_HPP_INCLUDE
