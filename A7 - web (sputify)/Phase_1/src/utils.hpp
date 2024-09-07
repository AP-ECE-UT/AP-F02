#ifndef UTILS_HPP_INCLUDE
#define UTILS_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <string>

#include "consts.hpp"
#include "types.hpp"

class Utils {
public:
    static inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
    }

    static inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
    }

    static inline void trim(std::string& s) {
        rtrim(s);
        ltrim(s);
    }

    static inline hh_mm_ss addHhMmSs(const hh_mm_ss& first, const hh_mm_ss& second) {
        std::chrono::seconds result_secs = first.seconds() + second.seconds();
        std::chrono::minutes result_minutes = first.minutes() + second.minutes();
        std::chrono::hours result_hours = first.hours() + second.hours();
        return hh_mm_ss(result_hours + result_minutes + result_secs);
    }

    static inline std::string hhMmSsToString(const hh_mm_ss& h) {
        return std::format(
            "{:02d}:{:02d}:{:02d}",
            h.hours().count(), h.minutes().count(), h.seconds().count());
    }

    static inline std::vector<std::string> readTags(std::string tags_str) {
        std::vector<std::string> tags;
        std::string tag;
        std::istringstream iss(tags_str);
        while (std::getline(iss, tag, consts::input::TAG_DELIM))
            tags.push_back(tag);
        return tags;
    }

    static inline hh_mm_ss readDuration(std::string duration_str) {
        std::string hour_str, minute_str, second_str;
        std::istringstream iss(duration_str);
        std::getline(iss, hour_str, consts::input::TIME_DELIM);
        std::getline(iss, minute_str, consts::input::TIME_DELIM);
        std::getline(iss, second_str, consts::input::TIME_DELIM);
        int hour = std::stoi(hour_str);
        int minute = std::stoi(minute_str);
        int second = std::stoi(second_str);
        std::chrono::seconds total_seconds = std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
        hh_mm_ss duration(total_seconds);
        return duration;
    }

    static inline std::ostringstream& addHeader(std::ostringstream& oss, const std::vector<std::string>& header) {
        for (std::size_t i = 0; i < header.size(); i++) {
            oss << header[i];
            if (i != header.size() - 1)
                oss << consts::output::INFO_DELIM;
        }
        oss << std::endl;
        return oss;
    }

    template <typename T>
    static inline std::ostringstream& addVectorLongInfo(std::ostringstream& oss, const std::vector<T>& vec) {
        for (std::size_t i = 0; i < vec.size(); i++) {
            oss << vec[i]->longInfoToString();
            if (i != vec.size() - 1)
                oss << std::endl;
        }
        return oss;
    }

    template <typename T>
    static inline std::ostringstream& addVectorShortInfo(std::ostringstream& oss, const std::vector<T>& vec) {
        for (std::size_t i = 0; i < vec.size(); i++) {
            oss << vec[i]->shortInfoToString();
            if (i != vec.size() - 1)
                oss << std::endl;
        }
        return oss;
    }

    template <typename T>
    static inline std::ostringstream& addVectorNames(std::ostringstream& oss, const std::vector<T>& vec) {
        for (std::size_t i = 0; i < vec.size(); i++) {
            oss << vec[i]->getName();
            if (i != vec.size() - 1)
                oss << consts::output::INFO_DELIM;
        }
        return oss;
    }
};

#endif // UTILS_HPP_INCLUDE
