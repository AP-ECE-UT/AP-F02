#include "cli_interface.hpp"

#include <iostream>
#include <stdexcept>

#include "consts.hpp"
#include "filter/color_manip/grayscale.hpp"
#include "filter/color_manip/invert.hpp"
#include "filter/filter.hpp"
#include "filter/kernel/blur.hpp"
#include "filter/kernel/emboss.hpp"
#include "filter/kernel/sharpen.hpp"
#include "utils.hpp"

const std::unordered_map<std::string, CliInterface::FilterType> CliInterface::filterFlagMap_{
    {"G", FilterType::grayscale},
    {"I", FilterType::invert},
    {"B", FilterType::blur},
    {"S", FilterType::sharpen},
    {"E", FilterType::emboss},
};

CliInterface::CliInterface(const std::vector<std::string>& flags) {
    for (unsigned i = 0; i < flags.size(); ++i) {
        if (flags[i][0] == '-') {
            Filter* f = createFilter(flags[i].substr(1));
            if (i != flags.size() - 1 && flags[i + 1][0] != '-') {
                ViewInfo vi = createViewInfo(flags[i + 1]);
                editor.addFilter(f, vi);
                ++i;
            }
            else {
                editor.addFilter(f);
            }
        }
    }
}

void CliInterface::run() {
    std::string cmd;
    while (std::getline(std::cin, cmd)) {
        std::vector<std::string> paths = utils::split(cmd, ' ');
        editor.editImage(paths[0], paths[1]);
    }
}

Filter* CliInterface::createFilter(const std::string& filterType) {
    auto itr = filterFlagMap_.find(filterType);
    if (itr == filterFlagMap_.end()) {
        throw std::invalid_argument("Invalid filter flag.");
    }
    switch (itr->second) {
    case FilterType::grayscale:
        return new GrayScale();
    case FilterType::invert:
        return new Invert();
    case FilterType::blur:
        return new Blur();
    case FilterType::sharpen:
        return new Sharpen();
    case FilterType::emboss:
        return new Emboss();
    }
    return nullptr;
}

ViewInfo CliInterface::createViewInfo(const std::string& viewInfo) {
    std::vector<std::string> viewParams = utils::split(viewInfo, consts::VIEW_DELIMITER);
    return ViewInfo{stoi(viewParams[1]),
                    stoi(viewParams[0]),
                    stoi(viewParams[2]),
                    stoi(viewParams[3])};
}
