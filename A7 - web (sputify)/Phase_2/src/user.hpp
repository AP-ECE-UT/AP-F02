#ifndef USER_HPP_INCLUDE
#define USER_HPP_INCLUDE

#include <string>

#include "playlist.hpp"
#include "types.hpp"

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
    virtual void removePlaylist(const std::string& name) = 0;
    virtual Music* createMusic(const std::string& title, const std::string& path, const std::string& year,
                               const std::string& album, const std::vector<std::string>& tags,
                               const hh_mm_ss& duration) = 0;
    virtual std::vector<PlayList*> getPlaylists() const = 0;
    virtual PlayList* getPlaylist(const std::string& name) const = 0;
    virtual std::vector<Music*> getMusics() const = 0;
    virtual std::vector<Music*> getMusics(const std::string& sort_by) const = 0;
    virtual std::vector<Music*> getLikedMusics() const = 0;
    virtual bool hasLiked(Music* music) const = 0;
    virtual void like(Music* music) = 0;
    virtual void follow(User* user);
    virtual void unfollow(User* user);

    virtual std::string shortInfoToString() const = 0;
    virtual std::string longInfoToString() const = 0;

protected:
    int id_;
    std::string username_;
    std::string password_;
    std::vector<User*> followings_;
    std::vector<User*> followers_;
    void addFollower(User* user);
    void removeFollower(User* user);
};

#endif // USER_HPP_INCLUDE
