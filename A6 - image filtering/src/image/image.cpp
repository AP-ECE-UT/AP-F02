#include "image.hpp"

Image::Image(const Image& other) {
    data_.assign(other.height(), std::vector<Pixel>(other.width()));
    for (int row = 0; row < other.height(); ++row) {
        for (int col = 0; col < other.width(); ++col) {
            data_[row][col] = other(row, col);
        }
    }
}

Image& Image::operator=(const Image& rhs) {
    Image temp(rhs);
    swap(*this, temp);
    return *this;
}

void swap(Image& a, Image& b) {
    using std::swap;
    swap(a.data_, b.data_);
}

bool Image::create(int width, int height) { return false; }
Image::ReadResult Image::read(const std::string& filename) { return ReadResult::unsupported_img; }
bool Image::write(const std::string& filename) { return false; }

int Image::width() const { return data_.front().size(); }
int Image::height() const { return data_.size(); }

Pixel Image::operator()(int row, int col) const {
    return data_[row][col];
}

Pixel& Image::operator()(int row, int col) {
    return data_[row][col];
}
