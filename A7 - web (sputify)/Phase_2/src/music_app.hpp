#ifndef MUSIC_APP_HPP_INCLUDE
#define MUSIC_APP_HPP_INCLUDE

#include "artist.hpp"
#include "data_manager.hpp"
#include "listener.hpp"

class MusicApp {
public:
    MusicApp(MusicApp& other) = delete;
    ~MusicApp();
    void operator=(const MusicApp& other) = delete;
    static MusicApp* getInstance();

    void signup(std::string& username, std::string& password, std::string& mode);
    void login(std::string& username, std::string& password);
    void logout();
    void createPlaylist(std::string& name);
    void removePlaylist(std::string& name);
    void addMusicToPlaylist(std::string& name, int music_id);
    void createMusic(std::string& title, std::string& path, std::string& year, std::string& album,
                     std::vector<std::string> tags, hh_mm_ss duration);
    void removeMusic(int id);
    void follow(int user_id);
    void unfollow(int user_id);
    void like(int music_id);

    std::vector<Music*> getAllMusics();
    std::vector<Music*> searchMusics(const std::string& name = "",
                                     const std::string& artist = "",
                                     const std::string& tag = "");
    std::vector<Music*> getArtistMusics();
    std::vector<Music*> getArtistMusics(const std::string& sort_by);
    std::vector<Music*> getLikedMusics();
    std::vector<Music*> getRecommendations();
    std::vector<User*> getAllUsers();
    std::vector<PlayList*> getUserPlaylists(int id);
    std::vector<Music*> getPlaylist(const int user_id, const std::string& name);
    User* getUser(int id);
    Music* getMusic(int id);

private:
    User* current_user_;
    DataManager* data_manager_;

    static MusicApp* music_app_;
    MusicApp();
};

#endif // MUSIC_APP_HPP_INCLUDE
