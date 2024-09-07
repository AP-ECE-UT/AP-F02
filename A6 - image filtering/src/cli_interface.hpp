#ifndef CLI_HPP_INCLUDE
#define CLI_HPP_INCLUDE

#include <string>
#include <unordered_map>
#include <vector>

#include "image/image_view.hpp"
#include "image_editor.hpp"

class Filter;

class CliInterface {
public:
    enum class FilterType {
        grayscale,
        invert,
        blur,
        sharpen,
        emboss,
    };

    CliInterface(const std::vector<std::string>& flags);
    void run();

private:
    static const std::unordered_map<std::string, FilterType> filterFlagMap_;

    ImageEditor editor;

    FilterType getFilterType(const std::string& filterFlag);
    Filter* createFilter(const std::string& filterType);
    ViewInfo createViewInfo(const std::string& viewInfo);
};

#endif // CLI_HPP_INCLUDE
