#ifndef IMAGE_EDITOR_HPP_INCLUDE
#define IMAGE_EDITOR_HPP_INCLUDE

#include <string>
#include <vector>

#include "image/image_view.hpp"

class Filter;

class ImageEditor {
public:
    ImageEditor() = default;
    ~ImageEditor();

    void addFilter(Filter* filter);
    void addFilter(Filter* filter, ViewInfo view);

    void editImage(const std::string& filename, const std::string& output);

private:
    std::vector<Filter*> filterPipe_;
    std::vector<ViewInfo> viewPipe_;
};

#endif // IMAGE_EDITOR_HPP_INCLUDE
