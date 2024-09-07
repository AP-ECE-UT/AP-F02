#include "request_handlers.hpp"

#include <sstream>
#include <string>

#include "consts.hpp"

SignupRequestHandler::SignupRequestHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse SignupRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string username = request.getArgValue(consts::arg_names::SIGNUP_USERNAME, consts::necessity::NECESSARY);
    std::string password = request.getArgValue(consts::arg_names::SIGNUP_PASSWORD, consts::necessity::NECESSARY);
    std::string mode = request.getArgValue(consts::arg_names::SIGNUP_MODE, consts::necessity::NECESSARY);

    app_->signup(username, password, mode);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

LoginRequestHandler::LoginRequestHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse LoginRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string username = request.getArgValue(consts::arg_names::LOGIN_USERNAME, consts::necessity::NECESSARY);
    std::string password = request.getArgValue(consts::arg_names::LOGIN_PASSWORD, consts::necessity::NECESSARY);

    app_->login(username, password);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

LogoutRequestHandler::LogoutRequestHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse LogoutRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    app_->logout();
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

GetMusicsHandler::GetMusicsHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse GetMusicsHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string id_str = request.getArgValue(consts::arg_names::ID, consts::necessity::UNNECESSARY);

    if (id_str == consts::input::NONE)
        return getAllMusics_();
    return getMusicInfo_(std::stoi(id_str));
}

CommandRequestHandler::CommandResponse GetMusicsHandler::getAllMusics_() {
    std::vector<Music*> musics = app_->getAllMusics();
    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST});
    Utils::addVectorShortInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse GetMusicsHandler::getMusicInfo_(int id) {
    Music* music = app_->getMusic(id);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST,
                           consts::output::YEAR, consts::output::ALBUM, consts::output::TAGS, consts::output::DURATION});
    oss << music->longInfoToString();
    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

GetUsersHandler::GetUsersHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse GetUsersHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string id_str = request.getArgValue(consts::arg_names::ID, consts::necessity::UNNECESSARY);

    if (id_str == consts::input::NONE)
        return getAllUsers_();
    return getUserInfo_(std::stoi(id_str));
}

CommandRequestHandler::CommandResponse GetUsersHandler::getAllUsers_() {
    std::vector<User*> users = app_->getAllUsers();
    if (users.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::MODE, consts::output::USERNAME, consts::output::PLAYLISTS_SONGS_NUM});
    Utils::addVectorShortInfo(oss, users);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse GetUsersHandler::getUserInfo_(int id) {
    User* user = app_->getUser(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, user->longInfoToString());
}

CreatePlaylistHandler::CreatePlaylistHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse CreatePlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::NECESSARY);

    app_->createPlaylist(name);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

AddSongToPlaylistHandler::AddSongToPlaylistHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse AddSongToPlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::NECESSARY);
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->addMusicToPlaylist(name, id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

GetUserPlaylistsHandler::GetUserPlaylistsHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse GetUserPlaylistsHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));
    std::vector<PlayList*> playlists = app_->getUserPlaylists(id);
    if (playlists.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::PLAYLIST_NAME, consts::output::SONGS_NUMBER, consts::output::DURATION});
    Utils::addVectorShortInfo(oss, playlists);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

SearchMusicHandler::SearchMusicHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse SearchMusicHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::UNNECESSARY);
    std::string artist = request.getArgValue(consts::arg_names::ARTIST, consts::necessity::UNNECESSARY);
    std::string tag = request.getArgValue(consts::arg_names::TAG, consts::necessity::UNNECESSARY);

    std::vector<Music*> musics = app_->searchMusics(name, artist, tag);
    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST});
    Utils::addVectorShortInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CreateMusicHandler::CreateMusicHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse CreateMusicHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string title = request.getArgValue(consts::arg_names::TITLE, consts::necessity::NECESSARY);
    std::string path = request.getArgValue(consts::arg_names::PATH, consts::necessity::NECESSARY);
    std::string year = request.getArgValue(consts::arg_names::YEAR, consts::necessity::NECESSARY);
    std::string album = request.getArgValue(consts::arg_names::ALBUM, consts::necessity::NECESSARY);
    std::vector<std::string> tags = Utils::readTags(request.getArgValue(consts::arg_names::TAGS, consts::necessity::NECESSARY));
    hh_mm_ss duration = Utils::readDuration(request.getArgValue(consts::arg_names::DURATION, consts::necessity::NECESSARY));

    app_->createMusic(title, path, year, album, tags, duration);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

RemoveMusicHandler::RemoveMusicHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse RemoveMusicHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->removeMusic(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

GetArtistMusicsHandler::GetArtistMusicsHandler(MusicApp* app)
    : CommandRequestHandler(app) {}

CommandRequestHandler::CommandResponse GetArtistMusicsHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::vector<Music*> musics = app_->getArtistMusics();
    if (musics.empty()) {
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);
    }

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST,
                           consts::output::YEAR, consts::output::ALBUM, consts::output::TAGS, consts::output::DURATION});
    Utils::addVectorLongInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}
