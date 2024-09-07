#include "command_request_handler.hpp"

#include <iostream>

#include "consts.hpp"
#include "exceptions.hpp"
#include "utils.hpp"

CommandRequestHandler::CommandRequestHandler(MusicApp* app) : app_(app) {}

CommandRequestHandler::CommandRequest::CommandRequest(std::istringstream& input_stream) {
    auto get_arg = [](std::istringstream& str) -> std::string {
        std::string result;
        str >> result;
        return result;
    };

    auto get_val = [](std::istringstream& str) -> std::string {
        std::string result;
        std::getline(str, result, consts::input::CMD_ARG_END);
        Utils::trim(result);
        if (result[0] != consts::input::CMD_ARG_START)
            throw new exceptions::BadRequest();
        return result.substr(1, result.size() - 1);
    };

    std::string current_line;
    while (std::getline(input_stream, current_line, consts::input::CMD_ARG_END)) {
        std::istringstream current_line_str(current_line);
        std::string arg = get_arg(current_line_str);
        std::string val = get_val(current_line_str);
        arg_vals_[arg] = val;
    }
}

std::string CommandRequestHandler::CommandRequest::getArgValue(const std::string arg, bool is_required) const {
    const auto& found_arg = arg_vals_.find(arg);
    if (found_arg == arg_vals_.end()) {
        if (is_required)
            throw new exceptions::BadRequest();
        return consts::input::NONE;
    }
    return found_arg->second;
}

CommandRequestHandler::CommandResponse::CommandResponse(StatusCode code, const std::string& body)
    : code_(code), body_(body) {}

CommandRequestHandler::CommandResponse::StatusCode CommandRequestHandler::CommandResponse::getCode() const {
    return code_;
}

std::string CommandRequestHandler::CommandResponse::getBody() const {
    return body_;
}

CommandRequestHandler::CommandResponse CommandRequestHandler::callbackSafe(const CommandRequestHandler::CommandRequest& request) {
    try {
        return callback(request);
    }
    catch (const exceptions::NotFound& e) {
        return CommandResponse(CommandResponse::StatusCode::NotFound, consts::responses::NOT_FOUND);
    }
    catch (const exceptions::PermissionDenied& e) {
        return CommandResponse(CommandResponse::StatusCode::PermissionDenied, consts::responses::PERMISSION_DENIED);
    }
    catch (...) {
        return CommandResponse(CommandResponse::StatusCode::BadRequest, consts::responses::BAD_REQUEST);
    }
}
