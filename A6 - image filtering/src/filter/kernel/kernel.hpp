#ifndef KERNEL_HPP_INCLUDE
#define KERNEL_HPP_INCLUDE

#include <array>

#include "../filter.hpp"

class Kernel : public Filter {
public:
    using Matrix = std::array<float, 9>;

    Kernel(Matrix km, bool normalize);

    void apply(Image* img) override;

protected:
    Matrix km_;

private:
    static Matrix normalizeMatrix(Matrix m);
};

#endif // KERNEL_HPP_INCLUDE
