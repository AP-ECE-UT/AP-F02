#include "io_handler.hpp"

#include <ios>
#include <string>

#include "consts.hpp"
#include "exceptions.hpp"
#include "music_app.hpp"
#include "request_handlers.hpp"
#include "utils.hpp"

IoHandler::IoHandler(std::istream& input_stream, std::ostream& output_stream)
    : input_stream_(input_stream), output_stream_(output_stream) {
    MusicApp* app = MusicApp::getInstance();
    // clang-format off
    cmds_ = {
        {consts::input::POST   + " " + consts::cmds::SIGNUP,            new SignupRequestHandler(app)},
        {consts::input::POST   + " " + consts::cmds::LOGIN,             new LoginRequestHandler(app)},
        {consts::input::POST   + " " + consts::cmds::LOGOUT,            new LogoutRequestHandler(app)},
        {consts::input::GET    + " " + consts::cmds::MUSICS,            new GetMusicsHandler(app)},
        {consts::input::GET    + " " + consts::cmds::USERS,             new GetUsersHandler(app)},
        {consts::input::POST   + " " + consts::cmds::PLAYLIST,          new CreatePlaylistHandler(app)},
        {consts::input::PUT    + " " + consts::cmds::ADD_PLAYLIST,      new AddSongToPlaylistHandler(app)},
        {consts::input::GET    + " " + consts::cmds::PLAYLIST,          new GetUserPlaylistsHandler(app)},
        {consts::input::GET    + " " + consts::cmds::SEARCH_MUSICS,     new SearchMusicHandler(app)},
        {consts::input::POST   + " " + consts::cmds::MUSIC,             new CreateMusicHandler(app)},
        {consts::input::DELETE + " " + consts::cmds::MUSIC,             new RemoveMusicHandler(app)},
        {consts::input::GET    + " " + consts::cmds::REGISTERED_MUSICS, new GetArtistMusicsHandler(app)},
    };
    // clang-format on
}

void IoHandler::run() {
    std::string cmd;
    while (std::getline(input_stream_, cmd, consts::input::CMD_DELIM)) {
        std::string arg_vals;
        std::getline(input_stream_, arg_vals);

        CommandRequestHandler* handler;
        try {
            handler = findHandler(cmd);
        }
        catch (const exceptions::BadRequest& e) {
            output_stream_ << e.what() << std::endl;
            continue;
        }
        catch (const exceptions::NotFound& e) {
            output_stream_ << e.what() << std::endl;
            continue;
        }

        CommandRequestHandler::CommandRequest request = makeRequest(arg_vals);
        output_stream_ << handler->callbackSafe(request).getBody() << std::endl;
    }
}

CommandRequestHandler* IoHandler::findHandler(std::string& cmd) {
    Utils::trim(cmd);
    if (!isMethodTypeValid(cmd))
        throw exceptions::BadRequest();
    const auto& found_handler = cmds_.find(cmd);
    if (found_handler == cmds_.end())
        throw exceptions::NotFound();

    return found_handler->second;
}

CommandRequestHandler::CommandRequest IoHandler::makeRequest(std::string& arg_vals) {
    Utils::trim(arg_vals);
    std::istringstream arg_vals_str(arg_vals);
    return CommandRequestHandler::CommandRequest(arg_vals_str);
}

bool IoHandler::isMethodTypeValid(std::string& cmd) {
    std::string method_type;
    std::istringstream cmd_str(cmd);
    cmd_str >> method_type;
    return method_type == consts::input::GET || method_type == consts::input::POST ||
           method_type == consts::input::PUT || method_type == consts::input::DELETE;
}
