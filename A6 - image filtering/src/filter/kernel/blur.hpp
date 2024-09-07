#ifndef BLUR_HPP_INCLUDE
#define BLUR_HPP_INCLUDE

#include "kernel.hpp"

class Blur : public Kernel {
public:
    Blur() : Kernel({1, 2, 1, 2, 4, 2, 1, 2, 1}, true) {}
};

#endif // BLUR_HPP_INCLUDE
