#include "listener.hpp"

#include <algorithm>
#include <sstream>

#include "consts.hpp"
#include "exceptions.hpp"
#include "music.hpp"
#include "playlist.hpp"

Listener::Listener(const std::string& username, const std::string& password)
    : User(username, password) {}

void Listener::addPlayList_(PlayList* playlist) {
    playlists_.push_back(playlist);
}

void Listener::createPlaylist(const std::string& name) {
    if (findPlaylistByName_(name) != nullptr)
        throw exceptions::BadRequest();
    addPlayList_(new PlayList(name, this));
}

void Listener::addMusicToPlaylist(const std::string& name, Music* music) {
    PlayList* playlist = findPlaylistByName_(name);
    if (playlist == nullptr)
        throw exceptions::NotFound();
    playlist->addMusic(music);
}

void Listener::removeMusic(Music* music) {
    for (auto playlist : playlists_)
        playlist->removeMusic(music);
}

void Listener::removePlaylist(const std::string& name) {
    auto it = std::find_if(playlists_.begin(), playlists_.end(), [&](PlayList* playlist) {
        return *playlist == name;
    });

    if (it == playlists_.end())
        throw exceptions::NotFound();

    playlists_.erase(it);
}

bool Listener::hasLiked(Music* music) const {
    for (auto m : likes_) {
        if (m == music)
            return true;
    }
    return false;
}

void Listener::like(Music* music) {
    if (hasLiked(music))
        throw exceptions::BadRequest();
    likes_.push_back(music);
    music->addLike();
}

std::string Listener::shortInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << getID()               << consts::output::INFO_DELIM
       << consts::output::LISTENER_MODE << consts::output::INFO_DELIM
       << getName()         << consts::output::INFO_DELIM
       << playlists_.size();
    // clang-format on
    return ss.str();
}

std::vector<PlayList*> Listener::getPlaylists() const {
    auto sorted_playlists = std::vector<PlayList*>(playlists_);
    sort(sorted_playlists.begin(), sorted_playlists.end(), [](PlayList* a, PlayList* b) {
        return a->getName() < b->getName();
    });
    return sorted_playlists;
}

PlayList* Listener::getPlaylist(const std::string& name) const {
    PlayList* playlist = findPlaylistByName_(name);
    if (playlist == nullptr)
        throw exceptions::NotFound();
    return playlist;
}

std::vector<Music*> Listener::getMusics() const {
    throw exceptions::PermissionDenied();
}

std::vector<Music*> Listener::getMusics(const std::string& sort_by) const {
    throw exceptions::PermissionDenied();
}

std::vector<Music*> Listener::getLikedMusics() const {
    return likes_;
}

Music* Listener::createMusic(const std::string& title, const std::string& path, const std::string& year,
                             const std::string& album, const std::vector<std::string>& tags,
                             const hh_mm_ss& duration) {
    throw exceptions::PermissionDenied();
}

std::string Listener::longInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << consts::output::ID        << consts::output::OUTPUT_INDICATOR << getID()                       << std::endl
       << consts::output::MODE      << consts::output::OUTPUT_INDICATOR << consts::output::LISTENER_MODE << std::endl
       << consts::output::USERNAME  << consts::output::OUTPUT_INDICATOR << getName()                     << std::endl;
    // clang-format on

    ss << consts::output::FOLLOWINGS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, followings_);

    ss << consts::output::FOLLOWERS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, followers_);

    ss << consts::output::PLAYLISTS << consts::output::OUTPUT_INDICATOR;
    Utils::addVectorNames(ss, playlists_);

    return ss.str();
}

PlayList* Listener::findPlaylistByName_(const std::string& name) const {
    auto it = std::find_if(playlists_.begin(), playlists_.end(), [&](PlayList* playlist) {
        return *playlist == name;
    });

    if (it != playlists_.end())
        return *it;
    return nullptr;
}
