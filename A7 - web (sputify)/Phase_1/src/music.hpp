#ifndef MUSIC_HPP_INCLUDE
#define MUSIC_HPP_INCLUDE

#include <string>
#include <vector>

#include "types.hpp"

class Artist;

class Music {
public:
    Music() = delete;
    Music(const Artist* creator,
          const std::string& file_name,
          const std::string& title,
          const std::string& year,
          const std::string& album,
          const std::vector<std::string>& tags,
          const hh_mm_ss& duration);

    int getID() const;
    std::string getArtistName() const;
    std::string getName() const;
    hh_mm_ss getDuration() const;
    void setID(int id);
    bool matchesFilters(const std::string& name = "",
                        const std::string& artist = "",
                        const std::string& tag = "") const;

    std::string shortInfoToString() const;
    std::string longInfoToString() const;

private:
    bool hasTag_(const std::string& tag) const;
    std::string convertTagsToString_() const;
    int id_;
    const Artist* creator_;
    std::string file_name_;
    struct {
        std::string title;
        std::string year;
        std::string album;
        std::vector<std::string> tags;
        hh_mm_ss duration;
    } info_;
};

#endif // MUSIC_HPP_INCLUDE
