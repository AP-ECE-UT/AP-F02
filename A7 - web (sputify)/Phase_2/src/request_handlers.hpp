#ifndef REQUEST_HANDLERS_HPP_INCLUDE
#define REQUEST_HANDLERS_HPP_INCLUDE

#include "command_request_handler.hpp"
#include "utils.hpp"

class SignupRequestHandler : public CommandRequestHandler {
public:
    SignupRequestHandler(MusicApp* app) : CommandRequestHandler(app) {}
    CommandResponse callback(const CommandRequest& request);
};

class LoginRequestHandler : public CommandRequestHandler {
public:
    LoginRequestHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class LogoutRequestHandler : public CommandRequestHandler {
public:
    LogoutRequestHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class GetMusicsHandler : public CommandRequestHandler {
public:
    GetMusicsHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;

private:
    CommandResponse getAllMusics_();
    CommandResponse getMusicInfo_(int id);
};

class GetUsersHandler : public CommandRequestHandler {
public:
    GetUsersHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;

private:
    CommandResponse getAllUsers_();
    CommandResponse getUserInfo_(int id);
};

class CreatePlaylistHandler : public CommandRequestHandler {
public:
    CreatePlaylistHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class AddSongToPlaylistHandler : public CommandRequestHandler {
public:
    AddSongToPlaylistHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class GetPlaylistHandler : public CommandRequestHandler {
public:
    GetPlaylistHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;

private:
    CommandResponse getUserPlaylists_(int id);
    CommandResponse getPlaylist(int id, std::string& name);
};

class DeletePlaylistHandler : public CommandRequestHandler {
public:
    DeletePlaylistHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class SearchMusicHandler : public CommandRequestHandler {
public:
    SearchMusicHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class CreateMusicHandler : public CommandRequestHandler {
public:
    CreateMusicHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class RemoveMusicHandler : public CommandRequestHandler {
public:
    RemoveMusicHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class GetArtistMusicsHandler : public CommandRequestHandler {
public:
    GetArtistMusicsHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class FollowHandler : public CommandRequestHandler {
public:
    FollowHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class UnfollowHandler : public CommandRequestHandler {
public:
    UnfollowHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class LikeHandler : public CommandRequestHandler {
public:
    LikeHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class GetLikesHandler : public CommandRequestHandler {
public:
    GetLikesHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

class GetRecommendationsHandler : public CommandRequestHandler {
public:
    GetRecommendationsHandler(MusicApp* app) : CommandRequestHandler(app) {}
    virtual CommandResponse callback(const CommandRequest& request) override;
};

#endif // REQUEST_HANDLERS_HPP_INCLUDE
