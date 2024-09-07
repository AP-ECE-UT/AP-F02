#ifndef IMAGE_HPP_INCLUDE
#define IMAGE_HPP_INCLUDE

#include <string>
#include <vector>

#include "pixel.hpp"

class Image {
public:
    Image() = default;
    virtual ~Image() = default;

    Image(const Image& other);
    Image& operator=(const Image& rhs);
    friend void swap(Image& a, Image& b);

    enum class ReadResult {
        success,
        open_err,
        invalid_file,
        unsupported_img,
        alloc_err,
    };

    virtual bool create(int width, int height);
    virtual ReadResult read(const std::string& filename);
    virtual bool write(const std::string& filename);

    virtual int width() const;
    virtual int height() const;

    virtual Pixel operator()(int row, int col) const;
    virtual Pixel& operator()(int row, int col);

protected:
    std::vector<std::vector<Pixel>> data_;
};

#endif // IMAGE_HPP_INCLUDE
