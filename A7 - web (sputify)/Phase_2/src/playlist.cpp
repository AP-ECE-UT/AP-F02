#include "playlist.hpp"

#include <algorithm>
#include <chrono>
#include <sstream>

#include "consts.hpp"
#include "utils.hpp"

PlayList::PlayList(const std::string& name, const Listener* creator)
    : name_(name), creator_(creator) {
    std::chrono::time_point<std::chrono::system_clock> initTime(std::chrono::seconds(0));
    duration_ = hh_mm_ss(initTime.time_since_epoch());
}

void PlayList::removeMusic(Music* music) {
    musics_.erase(std::remove(musics_.begin(), musics_.end(), music), musics_.end());
}

std::string PlayList::getName() const {
    return name_;
}

void PlayList::addMusic(Music* music) {
    musics_.push_back(music);
    duration_ = Utils::addHhMmSs(duration_, music->getDuration());
}

std::string PlayList::shortInfoToString() const {
    // clang-format off
    std::stringstream ss;
    ss << name_                 << consts::output::OUTPUT_DELIM
       << musics_.size()        << consts::output::OUTPUT_DELIM
       << Utils::hhMmSsToString(duration_);
    // clang-format on
    return ss.str();
}

bool PlayList::operator==(const std::string& name) const {
    return name_ == name;
}

std::vector<Music*> PlayList::getMusics() const {
    return musics_;
}