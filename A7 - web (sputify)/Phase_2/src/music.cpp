#include "music.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "artist.hpp"
#include "consts.hpp"
#include "exceptions.hpp"
#include "listener.hpp"
#include "user.hpp"
#include "utils.hpp"

Music::Music(const Artist* creator, const std::string& file_name, const std::string& title, const std::string& year,
             const std::string& album, const std::vector<std::string>& tags, const hh_mm_ss& duration)
    : creator_(creator), file_name_(file_name), info_({title, year, album, tags, duration}) {
    likes_ = 0;
}

std::string Music::convertTagsToString_() const {
    std::ostringstream ss;
    for (auto tag : info_.tags) {
        ss << tag;
        if (tag != info_.tags.back())
            ss << consts::output::TAG_DELIM;
    }
    return ss.str();
}

std::string Music::shortInfoWithLikesToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << id_         << consts::output::INFO_DELIM
       << info_.title << consts::output::INFO_DELIM
       << creator_->getName() << consts::output::INFO_DELIM
       << likes_;
    // clang-format on
    return ss.str();
}

std::string Music::shortInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << id_         << consts::output::INFO_DELIM
       << info_.title << consts::output::INFO_DELIM
       << creator_->getName();
    // clang-format on
    return ss.str();
}

std::string Music::longInfoToString() const {
    std::ostringstream ss;
    // clang-format off
    ss << id_                     << consts::output::INFO_DELIM
       << info_.title             << consts::output::INFO_DELIM
       << creator_->getName() << consts::output::INFO_DELIM
       << info_.year              << consts::output::INFO_DELIM
       << info_.album             << consts::output::INFO_DELIM
       << convertTagsToString_()  << consts::output::INFO_DELIM
       << Utils::hhMmSsToString(info_.duration);
    // clang-format on
    return ss.str();
}

int Music::getID() const {
    return id_;
}

std::string Music::getArtistName() const {
    return creator_->getName();
}

std::string Music::getName() const {
    return info_.title;
}

int Music::getLikes() const {
    return likes_;
}

hh_mm_ss Music::getDuration() const {
    return info_.duration;
}

void Music::setID(int id) {
    id_ = id;
}

bool Music::hasTag_(const std::string& tag) const {
    for (auto t : info_.tags) {
        if (t == tag)
            return true;
    }
    return false;
}

bool Music::matchesFilters(const std::string& name,
                           const std::string& artist,
                           const std::string& tag) const {
    if (name != consts::input::NONE && info_.title.find(name) == std::string::npos)
        return false;
    if (artist != consts::input::NONE && creator_->getName().find(artist) == std::string::npos)
        return false;
    if (tag != consts::input::NONE && !hasTag_(tag))
        return false;
    return true;
}

void Music::addLike() {
    likes_++;
}