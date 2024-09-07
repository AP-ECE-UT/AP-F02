#ifndef SHARPEN_HPP_INCLUDE
#define SHARPEN_HPP_INCLUDE

#include "kernel.hpp"

class Sharpen : public Kernel {
public:
    Sharpen() : Kernel({0, -1, 0, -1, 5, -1, 0, -1, 0}, false) {}
};

#endif // SHARPEN_HPP_INCLUDE
