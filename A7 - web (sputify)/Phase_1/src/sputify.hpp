#ifndef SPUTIFY_HPP_INCLUDE
#define SPUTIFY_HPP_INCLUDE

#include <iostream>

#include "io_handler.hpp"

class Sputify {
public:
    Sputify();
    void run();

private:
    IoHandler io_handler_;
};

#endif // SPUTIFY_HPP_INCLUDE
