#ifndef IO_HANDLER_HPP_INCLUDE
#define IO_HANDLER_HPP_INCLUDE

#include <ios>
#include <map>
#include <string>

#include "command_request_handler.hpp"
#include "music_app.hpp"
#include "request_handlers.hpp"

class IoHandler {
public:
    IoHandler() = delete;
    IoHandler(std::istream& input_stream, std::ostream& output_stream);

    void run();

private:
    // Command names are stored as "(GET|POST|PUT|DELETE) <cmd name>"
    std::map<std::string, CommandRequestHandler*> cmds_;

    std::istream& input_stream_;
    std::ostream& output_stream_;

    CommandRequestHandler* findHandler(std::string& cmd);
    bool isMethodTypeValid(std::string& cmd);
    CommandRequestHandler::CommandRequest makeRequest(std::string& arg_vals);
};

#endif // IO_HANDLER_HPP_INCLUDE
