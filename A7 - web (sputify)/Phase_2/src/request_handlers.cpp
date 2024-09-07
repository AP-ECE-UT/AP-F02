#include "request_handlers.hpp"

#include <sstream>
#include <string>

#include "consts.hpp"

CommandRequestHandler::CommandResponse SignupRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string username = request.getArgValue(consts::arg_names::SIGNUP_USERNAME, consts::necessity::NECESSARY);
    std::string password = request.getArgValue(consts::arg_names::SIGNUP_PASSWORD, consts::necessity::NECESSARY);
    std::string mode = request.getArgValue(consts::arg_names::SIGNUP_MODE, consts::necessity::NECESSARY);

    app_->signup(username, password, mode);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse LoginRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string username = request.getArgValue(consts::arg_names::LOGIN_USERNAME, consts::necessity::NECESSARY);
    std::string password = request.getArgValue(consts::arg_names::LOGIN_PASSWORD, consts::necessity::NECESSARY);

    app_->login(username, password);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse LogoutRequestHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    app_->logout();
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

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

CommandRequestHandler::CommandResponse CreatePlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::NECESSARY);

    app_->createPlaylist(name);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse AddSongToPlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::NECESSARY);
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->addMusicToPlaylist(name, id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse GetPlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::UNNECESSARY);

    if (name != consts::input::NONE) {
        return getPlaylist(id, name);
    }
    else {
        return getUserPlaylists_(id);
    }
}

CommandRequestHandler::CommandResponse GetPlaylistHandler::getPlaylist(int id, std::string& name) {
    std::vector<Music*> musics = app_->getPlaylist(id, name);
    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST});
    Utils::addVectorShortInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse GetPlaylistHandler::getUserPlaylists_(int id) {
    std::vector<PlayList*> playlists = app_->getUserPlaylists(id);
    if (playlists.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::PLAYLIST_NAME, consts::output::SONGS_NUMBER, consts::output::DURATION});
    Utils::addVectorShortInfo(oss, playlists);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse DeletePlaylistHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string name = request.getArgValue(consts::arg_names::NAME, consts::necessity::NECESSARY);

    app_->removePlaylist(name);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

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

CommandRequestHandler::CommandResponse RemoveMusicHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->removeMusic(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse GetArtistMusicsHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::string sort_by = request.getArgValue(consts::arg_names::SORT_BY, consts::necessity::UNNECESSARY);
    std::vector<Music*> musics;
    if (sort_by == consts::input::NONE)
        musics = app_->getArtistMusics();
    else
        musics = app_->getArtistMusics(sort_by);

    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST,
                           consts::output::YEAR, consts::output::ALBUM, consts::output::TAGS, consts::output::DURATION});
    Utils::addVectorLongInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse FollowHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->follow(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse UnfollowHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->unfollow(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse LikeHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    int id = std::stoi(request.getArgValue(consts::arg_names::ID, consts::necessity::NECESSARY));

    app_->like(id);
    return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::OK);
}

CommandRequestHandler::CommandResponse GetLikesHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::vector<Music*> musics = app_->getLikedMusics();
    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST});
    Utils::addVectorShortInfo(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}

CommandRequestHandler::CommandResponse GetRecommendationsHandler::callback(const CommandRequestHandler::CommandRequest& request) {
    std::vector<Music*> musics = app_->getRecommendations();
    if (musics.empty())
        return CommandResponse(CommandResponse::StatusCode::Ok, consts::responses::EMPTY);

    std::ostringstream oss;
    Utils::addHeader(oss, {consts::output::ID, consts::output::NAME, consts::output::ARTIST, consts::output::LIKES});
    Utils::addVectorShortInfoWithLikes(oss, musics);

    return CommandResponse(CommandResponse::StatusCode::Ok, oss.str());
}
