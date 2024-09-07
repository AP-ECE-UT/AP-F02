#ifndef EMBOSS_HPP_INCLUDE
#define EMBOSS_HPP_INCLUDE

#include "kernel.hpp"

class Emboss : public Kernel {
public:
    Emboss() : Kernel({-2, -1, 0, -1, 1, 1, 0, 1, 2}, false) {}
};

#endif // EMBOSS_HPP_INCLUDE
