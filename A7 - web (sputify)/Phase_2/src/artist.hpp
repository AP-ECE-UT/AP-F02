#ifndef ARTIST_HPP_INCLUDE
#define ARTIST_HPP_INCLUDE

#include <vector>

#include "consts.hpp"
#include "sstream"
#include "user.hpp"
#include "utils.hpp"

class Music;

class Artist : public User {
public:
    Artist() = delete;
    Artist(const std::string& username, const std::string& password);

    virtual void createPlaylist(const std::string& name) override;
    virtual void addMusicToPlaylist(const std::string& name, Music* music) override;
    virtual void removeMusic(Music* music) override;
    virtual void removePlaylist(const std::string& name) override;
    virtual Music* createMusic(const std::string& title, const std::string& path, const std::string& year,
                               const std::string& album, const std::vector<std::string>& tags,
                               const hh_mm_ss& duration) override;
    virtual std::vector<Music*> getMusics() const override;
    virtual std::vector<Music*> getMusics(const std::string& sort_by) const override;
    virtual std::vector<Music*> getLikedMusics() const override;
    virtual bool hasLiked(Music* music) const override;
    virtual std::vector<PlayList*> getPlaylists() const override;
    virtual PlayList* getPlaylist(const std::string& name) const override;
    virtual std::string shortInfoToString() const override;
    virtual std::string longInfoToString() const override;
    virtual void like(Music* music) override;

private:
    void addPublishedSong_(Music* music);
    std::vector<Music*> published_musics_;
};

#endif // ARTIST_HPP_INCLUDE
