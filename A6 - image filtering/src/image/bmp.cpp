#include "bmp.hpp"

#include <algorithm>
#include <fstream>

Bmp::Bmp() : Image() {}

Bmp::~Bmp() {
    delete[] fileData_;
}

Bmp::Bmp(const Bmp& other) {
    if (!other.valid_) return;
    delete[] fileData_;
    fileData_ = new char[other.hdr_.fileSize];
    std::copy(other.fileData_, other.fileData_ + other.hdr_.fileSize, fileData_);
    valid_ = true;
    hdr_ = other.hdr_;
    infoHdr_ = other.infoHdr_;
    padding_ = other.padding_;
    data_ = other.data_;
}

Bmp& Bmp::operator=(const Bmp& rhs) {
    Bmp temp(rhs);
    swap(*this, temp);
    return *this;
}

void swap(Bmp& a, Bmp& b) {
    using std::swap;
    swap(a.fileData_, b.fileData_);
    swap(a.valid_, b.valid_);
    swap(a.hdr_, b.hdr_);
    swap(a.infoHdr_, b.infoHdr_);
    swap(a.padding_, b.padding_);
    swap(a.data_, b.data_);
}

bool Bmp::create(int width, int height) {
    valid_ = false;
    padding_ = calcPadding(width);

    hdr_.fileType = bmp::FILE_TYPE;
    hdr_.fileSize = sizeof(bmp::Header) + sizeof(bmp::InfoHeader) + (width + padding_) * height * sizeof(Pixel);
    hdr_.reserved1 = 0;
    hdr_.reserved2 = 0;
    hdr_.offset = sizeof(bmp::Header) + sizeof(bmp::InfoHeader);

    delete[] fileData_;
    fileData_ = new (std::nothrow) char[hdr_.fileSize];
    if (fileData_ == nullptr) return false;
    std::fill(fileData_, fileData_ + hdr_.fileSize, 0x00);

    infoHdr_ = {0};
    infoHdr_.headerSize = sizeof(bmp::InfoHeader);
    infoHdr_.width = width;
    infoHdr_.height = height;
    infoHdr_.planes = 1;
    infoHdr_.bitCount = 24;
    infoHdr_.imageSize = (width + padding_) * height * sizeof(Pixel);

    std::copy((char*)&hdr_, (char*)&hdr_ + sizeof(bmp::Header), fileData_);
    std::copy((char*)&infoHdr_, (char*)&infoHdr_ + sizeof(bmp::InfoHeader), fileData_ + sizeof(bmp::Header));

    valid_ = true;
    data_.assign(height, std::vector<Pixel>(width));
    return true;
}

Image::ReadResult Bmp::read(const std::string& filename) {
    std::ifstream file(filename, std::ios_base::binary);
    if (!file.is_open()) return ReadResult::open_err;
    valid_ = false;

    file.read((char*)&hdr_, sizeof(bmp::Header));
    file.read((char*)&infoHdr_, sizeof(bmp::InfoHeader));

    if (hdr_.fileType != bmp::FILE_TYPE) return ReadResult::invalid_file;
    if (infoHdr_.bitCount != 24 || infoHdr_.compression != 0) return ReadResult::unsupported_img;
    padding_ = calcPadding(infoHdr_.width);

    file.seekg(0, std::ios_base::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios_base::beg);

    delete[] fileData_;
    fileData_ = new (std::nothrow) char[length];
    if (fileData_ == nullptr) return ReadResult::alloc_err;
    file.read(fileData_, length);

    valid_ = true;
    dataToVector();
    return ReadResult::success;
}

bool Bmp::write(const std::string& filename) {
    if (!valid_) return false;
    dataFromVector();

    std::ofstream file(filename, std::ios_base::binary);
    if (!file.is_open()) return false;

    file.write(fileData_, hdr_.fileSize);
    return true;
}

bool Bmp::valid() const { return valid_; }

int Bmp::calcPadding(int width) {
    const int bytesInRow = width * sizeof(Pixel);
    return bytesInRow % 4 ? 4 - bytesInRow % 4 : 0;
}

Pixel& Bmp::getPixel(int row, int col) {
    const int rowStart = (height() - 1 - row) * (width() * sizeof(Pixel) + padding_);
    char* const pixelPos = fileData_ + hdr_.offset + rowStart + col * sizeof(Pixel);
    return *(Pixel*)pixelPos;
}

void Bmp::dataToVector() {
    data_.assign(infoHdr_.height, std::vector<Pixel>(infoHdr_.width));
    for (int row = 0; row < height(); ++row) {
        for (int col = 0; col < width(); ++col) {
            data_[row][col] = getPixel(row, col);
        }
    }
}

void Bmp::dataFromVector() {
    for (int row = 0; row < height(); ++row) {
        for (int col = 0; col < width(); ++col) {
            getPixel(row, col) = data_[row][col];
        }
    }
}
