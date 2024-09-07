#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

constexpr char TIME_DELIM = ':';

using hh_mm = std::chrono::hh_mm_ss<std::chrono::system_clock::duration>;

struct SubwayStation {
    std::string name;
    std::chrono::minutes timeDistance;
};

struct SubwayLine {
    std::string name;
    std::vector<SubwayStation> toStartStations;
    std::vector<SubwayStation> toEndStations;

    std::chrono::minutes trainIntervalTime;
};

struct DestinationPath {
    const SubwayLine* subwayLine;
    int stationIndex;

    enum class Direction {
        towardsStart,
        towardsEnd
    } direction;
};

struct Inputs {
    hh_mm currentTime;
    std::vector<SubwayLine> subwayLines;
    std::string dest;
};

hh_mm parseCurrentTime(std::istream& inputStream) {
    int hour, minute;
    char delim;
    // "hh:mm" >> hour >> delim (":") >> minute;
    inputStream >> hour >> delim >> minute;
    std::chrono::time_point<std::chrono::system_clock> currentTimePoint(
        std::chrono::hours(hour) + std::chrono::minutes(minute));
    return std::chrono::hh_mm_ss(currentTimePoint.time_since_epoch());
}

SubwayStation parseSubwayStation(std::istream& inputStream) {
    int timeDistance;
    std::string name;
    inputStream >> timeDistance >> name;
    return {name, std::chrono::minutes(timeDistance)};
}

SubwayLine parseSubwayLine(std::istream& inputStream) {
    SubwayLine result;

    int trainIntervalTime;

    inputStream >> result.name >> trainIntervalTime;
    result.trainIntervalTime = std::chrono::minutes(trainIntervalTime);

    int startStationsCount;
    inputStream >> startStationsCount;
    while (startStationsCount--)
        result.toStartStations.push_back(parseSubwayStation(inputStream));

    int endStationCount;
    inputStream >> endStationCount;
    while (endStationCount--)
        result.toEndStations.push_back(parseSubwayStation(inputStream));

    return result;
}

Inputs parseInputs(std::istream& inputStream) {
    Inputs result;

    int subwayLinesCount;
    inputStream >> subwayLinesCount;

    result.currentTime = parseCurrentTime(inputStream);

    while (subwayLinesCount--)
        result.subwayLines.push_back(parseSubwayLine(inputStream));

    inputStream >> result.dest;

    return result;
}

DestinationPath findDest(const std::vector<SubwayLine>& subwayLines, const std::string& dest) {
    for (int i = 0; i < subwayLines.size(); ++i) {
        for (int s = 0; s < subwayLines[i].toStartStations.size(); ++s) {
            if (subwayLines[i].toStartStations[s].name == dest)
                return {&subwayLines[i], s, DestinationPath::Direction::towardsStart};
        }
        for (int e = 0; e < subwayLines[i].toEndStations.size(); ++e) {
            if (subwayLines[i].toEndStations[e].name == dest)
                return {&subwayLines[i], e, DestinationPath::Direction::towardsEnd};
        }
    }
    return {nullptr, -1, DestinationPath::Direction::towardsEnd};
}

hh_mm calcArrivalTime(const DestinationPath& path, const hh_mm& currentTime) {
    std::chrono::minutes travelTime(0);

    const auto& subwayLine =
        path.direction == DestinationPath::Direction::towardsEnd ? path.subwayLine->toEndStations : path.subwayLine->toStartStations;

    for (int i = 0; i <= path.stationIndex; ++i) {
        travelTime += subwayLine[i].timeDistance;
    }

    std::chrono::minutes minutes = currentTime.minutes() - currentTime.minutes() % path.subwayLine->trainIntervalTime;
    if (minutes < currentTime.minutes())
        minutes += path.subwayLine->trainIntervalTime;
    std::chrono::hours hours = currentTime.hours();
    return hh_mm(travelTime + minutes + hours);
}

int calcCost(int stationIndex) {
    return std::ceil(1000 * std::log10(10 * (stationIndex + 1)));
}

void printInfo(const DestinationPath& path, const hh_mm& arrival, std::ostream& outputStream) {
    outputStream << "Towards "
                 << ((path.direction == DestinationPath::Direction::towardsEnd) ? "end " : "start ")
                 << "of " << path.subwayLine->name << " "
                 << "in " << path.stationIndex + 1 << " station(s)" << std::endl;

    outputStream << std::setfill('0') << std::setw(2) << arrival.hours().count() % 24 << TIME_DELIM
                 << std::setfill('0') << std::setw(2) << arrival.minutes().count() << std::endl;
    outputStream << calcCost(path.stationIndex) << std::endl;
}

int main() {
    Inputs inputs = parseInputs(std::cin);
    DestinationPath path = findDest(inputs.subwayLines, inputs.dest);
    hh_mm arrival = calcArrivalTime(path, inputs.currentTime);
    printInfo(path, arrival, std::cout);
    return 0;
}
