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

void Artist::removeMusic(Music* music) {
    published_musics_.erase(std::remove(published_musics_.begin(), published_musics_.end(), music), published_musics_.end());
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

std::vector<Music*> Artist::getMusics() const {
    return published_musics_;
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
       << consts::output::USERNAME  << consts::output::OUTPUT_INDICATOR << getName()                     << std::endl
       << consts::output::SONGS     << consts::output::OUTPUT_INDICATOR;
    // clang-format on

    Utils::addVectorNames(ss, published_musics_);

    return ss.str();
}
