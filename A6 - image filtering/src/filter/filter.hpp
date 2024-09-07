#ifndef FILTER_HPP_INCLUDE
#define FILTER_HPP_INCLUDE

class Image;

class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;

    void setImage(Image* img);
    void apply();
    virtual void apply(Image* img) = 0;

protected:
    Image* img_ = nullptr;
};

#endif // FILTER_HPP_INCLUDE
