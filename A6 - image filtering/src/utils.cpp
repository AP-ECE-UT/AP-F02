#include "utils.hpp"

#include <algorithm>
#include <sstream>

namespace utils {

std::vector<std::string> split(const std::string& str, char delim) {
    std::istringstream sstr(str);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(sstr, item, delim)) {
        result.push_back(std::move(item));
    }
    return result;
}

} // namespace utils
