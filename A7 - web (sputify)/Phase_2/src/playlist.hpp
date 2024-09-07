#ifndef PLAYLIST_HPP_INCLUDE
#define PLAYLIST_HPP_INCLUDE

#include <string>
#include <vector>

#include "music.hpp"

class Listener;

class PlayList {
public:
    PlayList() = delete;
    PlayList(const std::string& name, const Listener* creator);

    void addMusic(Music* music);
    void removeMusic(Music* music);
    std::string shortInfoToString() const;
    std::string getName() const;
    bool operator==(const std::string& name) const;
    std::vector<Music*> getMusics() const;

private:
    std::string name_;
    std::vector<Music*> musics_;
    const Listener* creator_;

    hh_mm_ss duration_;
};

#endif // PLAYLIST_HPP_INCLUDE
