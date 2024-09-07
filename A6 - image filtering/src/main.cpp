#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "cli_interface.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Please provide filter flags.\n";
        return 1;
    }
    std::vector<std::string> flags(argv + 1, argv + argc);

    try {
        CliInterface cli(flags);
        cli.run();
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
    catch (const std::runtime_error& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
