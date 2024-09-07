#ifndef COMMAND_REQUEST_HANDLER_HPP_INCLUDE
#define COMMAND_REQUEST_HANDLER_HPP_INCLUDE

#include <map>
#include <sstream>
#include <string>

#include "music_app.hpp"

class CommandRequestHandler {
public:
    CommandRequestHandler(MusicApp* app);
    struct CommandRequest {
    public:
        CommandRequest(std::istringstream& input_stream);
        std::string getArgValue(const std::string arg, bool is_required) const;

    private:
        std::map<std::string, std::string> arg_vals_;
    };

    struct CommandResponse {
    public:
        enum class StatusCode {
            Ok,
            BadRequest,
            NotFound,
            PermissionDenied
        };

        CommandResponse(StatusCode code, const std::string& body);

        StatusCode getCode() const;
        std::string getBody() const;

    private:
        StatusCode code_;
        std::string body_;
    };

    // Uses callback to handle request but will also throw every exception
    // thrown by callback
    CommandResponse callbackSafe(const CommandRequest& request);

protected:
    virtual CommandResponse callback(const CommandRequest& request) = 0;
    MusicApp* app_;
};

#endif // COMMAND_REQUEST_HANDLER_HPP_INCLUDE
