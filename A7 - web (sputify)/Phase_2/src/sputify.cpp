#include "sputify.hpp"

Sputify::Sputify() : io_handler_(std::cin, std::cout) {}

void Sputify::run() {
    io_handler_.run();
}