#include "artist.hpp"

#include <algorithm>
#include <ostream>

#include "consts.hpp"
#include "exceptions.hpp"
#include "music.hpp"

Artist::Artist(const std::string& username, const std::string& password)
    : User(username, password) {}

void Artist::addPublishedSong_(Music* music) {
    published_musics_.push_back(music);
}

void Artist::createPlaylist(const std::string& name) {
    throw exceptions::PermissionDenied();
}

void Artist::addMusicToPlaylist(const std::string& name, Music* music) {
    throw exceptions::PermissionDenied();
}

void Artist::like(Music* music) {
    throw exceptions::PermissionDenied();
}

void Artist::removeMusic(Music* music) {
    published_musics_.erase(std::remove(published_musics_.begin(), published_musics_.end(), music), published_musics_.end());
}

void Artist::removePlaylist(const std::string& name) {
    throw exceptions::PermissionDenied();
}

Music* Artist::createMusic(const std::string& title, const std::string& path, const std::string& year,
                           const std::string& album, const std::vector<std::string>& tags,
                           const hh_mm_ss& duration) {
    Music* new_music = new Music(this, path, title, year, album, tags, duration);
    addPublishedSong_(new_music);
    return new_music;
}

std::vector<PlayList*> Artist::getPlaylists() const {
    throw exceptions::BadRequest();
}

PlayList* Artist::getPlaylist(const std::string& name) const {
    throw exceptions::BadRequest();
}

std::vector<Music*> Artist::getMusics() const {
    return published_musics_;
}

std::vector<Music*> Artist::getMusics(const std::string& sort_by) const {
    auto sorted_musics = std::vector<Music*>(published_musics_);
    if (sort_by == consts::input::SORTBY_ID)
        sort(sorted_musics.begin(), sorted_musics.end(), [](Music* a, Music* b) {
            return a->getID() < b->getID();
        });
    else if (sort_by == consts::input::SORTBY_LIKES)
        sort(sorted_musics.begin(), sorted_musics.end(), [](Music* a, Music* b) {
            return a->getLikes() == b->getLikes() ? a->getID() < b->getID() : a->getLikes() > b->getLikes();
        });
    else
        throw exceptions::BadRequest();

    return sorted_musics;
}

std::vector<Music*> Artist::getLikedMusics() const {
    throw exceptions::BadRequest();
}

bool Artist::hasLiked(Music* music) const {
    throw exceptions::BadRequest();
}

std::string Artist::shortInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << getID()                     << consts::output::INFO_DELIM
       << consts::output::ARTIST_MODE << consts::output::INFO_DELIM
       << getName()               << consts::output::INFO_DELIM
       << published_musics_.size();
    // clang-format on
    return ss.str();
}

#include <iostream>

std::string Artist::longInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << consts::output::ID        << consts::output::OUTPUT_INDICATOR << getID()                       << std::endl
       << consts::output::MODE      << consts::output::OUTPUT_INDICATOR << consts::output::ARTIST_MODE   << std::endl
       << consts::output::USERNAME  << consts::output::OUTPUT_INDICATOR << getName()                     << std::endl;
    // clang-format on

    ss << consts::output::FOLLOWINGS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, followings_);

    ss << consts::output::FOLLOWERS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, followers_);

    ss << consts::output::SONGS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, published_musics_);

    return ss.str();
}
