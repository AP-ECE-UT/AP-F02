#ifndef BMP_HPP_INCLUDE
#define BMP_HPP_INCLUDE

#include <cstdint>
#include <string>
#include <vector>

#include "image.hpp"
#include "pixel.hpp"

namespace bmp {

#pragma pack(push, 1)
struct Header {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};
struct InfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

constexpr uint16_t FILE_TYPE = 0x4D42;

} // namespace bmp

class Bmp : public Image {
public:
    Bmp();
    ~Bmp();

    Bmp(const Bmp& other);
    Bmp& operator=(const Bmp& rhs);
    friend void swap(Bmp& a, Bmp& b);

    bool create(int width, int height) override;
    ReadResult read(const std::string& filename) override;
    bool write(const std::string& filename) override;

    bool valid() const;

private:
    char* fileData_ = nullptr;
    bool valid_ = false;
    bmp::Header hdr_;
    bmp::InfoHeader infoHdr_;
    int padding_;

    static int calcPadding(int width);
    Pixel& getPixel(int row, int col);

    void dataToVector();
    void dataFromVector();
};

#endif // BMP_HPP_INCLUDE
