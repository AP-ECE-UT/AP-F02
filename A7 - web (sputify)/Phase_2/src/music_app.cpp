#include "music_app.hpp"

#include "consts.hpp"
#include "exceptions.hpp"

MusicApp* MusicApp::music_app_ = nullptr;

MusicApp* MusicApp::getInstance() {
    if (music_app_ == nullptr)
        music_app_ = new MusicApp();
    return music_app_;
}

MusicApp::MusicApp() {
    data_manager_ = new DataManager();
    current_user_ = nullptr;
}

MusicApp::~MusicApp() {
    delete data_manager_;
}

void MusicApp::signup(std::string& username, std::string& password, std::string& mode) {
    if (current_user_ != nullptr)
        throw exceptions::PermissionDenied();
    if (data_manager_->isUsernameTaken(username))
        throw exceptions::BadRequest();
    User* new_user;
    if (mode == consts::input::ARTIST_MODE)
        new_user = new Artist(username, password);
    else if (mode == consts::input::LISTENER_MODE)
        new_user = new Listener(username, password);
    data_manager_->addUser(new_user);
    current_user_ = new_user;
}

void MusicApp::login(std::string& username, std::string& password) {
    if (current_user_ != nullptr)
        throw exceptions::PermissionDenied();
    User* user;
    try {
        user = data_manager_->findUserByName(username);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
    if (!user->validatePassword(password))
        throw exceptions::PermissionDenied();
    current_user_ = user;
}

void MusicApp::logout() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    current_user_ = nullptr;
}

void MusicApp::createPlaylist(std::string& name) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        current_user_->createPlaylist(name);
    }
    catch (std::exception& e) {
        throw;
    }
}

void MusicApp::removePlaylist(std::string& name) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        current_user_->removePlaylist(name);
    }
    catch (std::exception& e) {
        throw;
    }
}

void MusicApp::createMusic(std::string& title, std::string& path, std::string& year, std::string& album,
                           std::vector<std::string> tags, hh_mm_ss duration) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        Music* new_music = current_user_->createMusic(title, path, year, album, tags, duration);
        data_manager_->addMusic(new_music);
    }
    catch (std::exception& e) {
        throw;
    }
}

void MusicApp::removeMusic(int id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        Music* music = data_manager_->findMusicByID(id);
        if (music->getArtistName() != current_user_->getName())
            throw exceptions::PermissionDenied();
        data_manager_->removeMusic(music);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

void MusicApp::addMusicToPlaylist(std::string& name, int music_id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        Music* music = data_manager_->findMusicByID(music_id);
        current_user_->addMusicToPlaylist(name, music);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

void MusicApp::follow(int user_id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        User* user = data_manager_->findUserByID(user_id);
        current_user_->follow(user);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

void MusicApp::unfollow(int user_id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        User* user = data_manager_->findUserByID(user_id);
        current_user_->unfollow(user);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

void MusicApp::like(int music_id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        Music* music = data_manager_->findMusicByID(music_id);
        current_user_->like(music);
    }
    catch (std::exception& e) {
        throw;
    }
}

std::vector<Music*> MusicApp::getAllMusics() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return data_manager_->getMusics();
}

std::vector<User*> MusicApp::getAllUsers() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return data_manager_->getAllUsers();
}

std::vector<PlayList*> MusicApp::getUserPlaylists(int id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        User* user = data_manager_->findUserByID(id);
        return user->getPlaylists();
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

std::vector<Music*> MusicApp::getPlaylist(const int user_id, const std::string& name) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        User* user = data_manager_->findUserByID(user_id);
        return user->getPlaylist(name)->getMusics();
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

std::vector<Music*> MusicApp::getRecommendations() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        std::vector<Music*> musics = data_manager_->getMusics();
        std::vector<Music*> userLikedMusics = current_user_->getLikedMusics();

        std::sort(musics.begin(), musics.end(), [](Music* a, Music* b) {
            return (a->getLikes() == b->getLikes()) ? a->getID() < b->getID() : a->getLikes() > b->getLikes();
        });

        std::vector<Music*> selected;
        std::copy_if(musics.begin(), musics.end(), std::back_inserter(selected), [&](Music* music) {
            return music->getLikes() > 0 && !current_user_->hasLiked(music);
        });

        selected.resize(std::min(selected.size(), consts::RECOMMENDATIONS_COUNT));
        return selected;
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

std::vector<Music*> MusicApp::searchMusics(const std::string& name, const std::string& artist, const std::string& tag) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return data_manager_->getMusics(name, artist, tag);
}

std::vector<Music*> MusicApp::getArtistMusics() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return current_user_->getMusics();
}

std::vector<Music*> MusicApp::getArtistMusics(const std::string& sort_by) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return current_user_->getMusics(sort_by);
}

std::vector<Music*> MusicApp::getLikedMusics() {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    return current_user_->getLikedMusics();
}

Music* MusicApp::getMusic(int id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        return data_manager_->findMusicByID(id);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}

User* MusicApp::getUser(int id) {
    if (current_user_ == nullptr)
        throw exceptions::PermissionDenied();
    try {
        return data_manager_->findUserByID(id);
    }
    catch (exceptions::NotFound& e) {
        throw;
    }
}
