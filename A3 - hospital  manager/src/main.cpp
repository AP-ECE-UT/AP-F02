#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace consts {

const std::string CSV_PATIENTS = "patients.csv";
const std::string CSV_DOCTORS = "doctors.csv";
const std::string CSV_DISEASES = "diseases.csv";

constexpr char DELIM_CSV = ',';
constexpr char DELIM_TIMES = '$';
constexpr char DELIM_DATE = '-';

} // namespace consts

struct Csv {
    std::vector<std::vector<std::string>> data;
    std::unordered_map<std::string, int> header;
};

enum class Weekday {
    Sat,
    Sun,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
};

std::unordered_map<std::string, Weekday> weekdayMap{
    {"Sat", Weekday::Sat},
    {"Sun", Weekday::Sun},
    {"Mon", Weekday::Mon},
    {"Tue", Weekday::Tue},
    {"Wed", Weekday::Wed},
    {"Thu", Weekday::Thu},
    {"Fri", Weekday::Fri},
};

std::unordered_map<Weekday, std::string> weekdayStringMap{
    {Weekday::Sat, "Sat"},
    {Weekday::Sun, "Sun"},
    {Weekday::Mon, "Mon"},
    {Weekday::Tue, "Tue"},
    {Weekday::Wed, "Wed"},
    {Weekday::Thu, "Thu"},
    {Weekday::Fri, "Fri"},
};

struct Time {
    Weekday day;
    int hour;
    int minute;
};

bool operator==(const Time& lhs, const Time& rhs) {
    return lhs.day == rhs.day && lhs.hour == rhs.hour && lhs.minute == rhs.minute;
}
bool operator!=(const Time& lhs, const Time& rhs) { return !(lhs == rhs); }
bool operator<(const Time& lhs, const Time& rhs) {
    if (lhs.day != rhs.day) {
        return lhs.day < rhs.day;
    }
    if (lhs.hour != rhs.hour) {
        return lhs.hour < rhs.hour;
    }
    return lhs.minute < rhs.minute;
}
bool operator>(const Time& lhs, const Time& rhs) { return rhs < lhs; }
bool operator<=(const Time& lhs, const Time& rhs) { return !(rhs < lhs); }
bool operator>=(const Time& lhs, const Time& rhs) { return !(lhs < rhs); }

struct Interval {
    Time start;
    Time end;
};

struct Doctor {
    std::string name;
    std::string specialty;
    int visitFee;
    int visitDuration;
    int waitTime;
    std::vector<Interval> available;
    std::vector<Interval> taken;
};

struct Reservation {
    Time start;
    Doctor* doc;
};

struct Patient {
    std::string name;
    std::string medicalIssue;
    Time appointed;
    Reservation res;
};

struct Data {
    std::vector<Patient> patients;
    std::vector<Doctor> doctors;
    std::unordered_map<std::string, std::string> issueToSpecialty;
};

std::vector<std::string> split(const std::string& str, char delim) {
    std::istringstream sstr(str);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(sstr, item, delim)) {
        result.push_back(std::move(item));
    }
    return result;
}

Csv readCsv(const std::string& filename) {
    Csv csv;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << '\n';
        std::exit(1);
    }

    std::string header;
    std::getline(file, header);
    auto parts = split(header, consts::DELIM_CSV);
    for (unsigned i = 0; i < parts.size(); ++i) {
        csv.header[parts[i]] = i;
    }

    std::string line;
    while (std::getline(file, line)) {
        csv.data.push_back(split(line, consts::DELIM_CSV));
    }
    return csv;
}

std::vector<Patient> initializePatients() {
    Csv csv = readCsv(consts::CSV_PATIENTS);
    std::vector<Patient> patients;

    for (const auto& row : csv.data) {
        Patient p{};
        p.name = row[csv.header["Name"]];
        p.medicalIssue = row[csv.header["Medical Issue"]];

        std::string madeAppointment = row[csv.header["Appointment Time"]];
        auto parts = split(madeAppointment, consts::DELIM_DATE);
        p.appointed.day = weekdayMap[parts[0]];
        p.appointed.hour = std::stoi(parts[1]);

        patients.push_back(p);
    }

    return patients;
}

std::vector<Doctor> initializeDoctors() {
    Csv csv = readCsv(consts::CSV_DOCTORS);
    std::vector<Doctor> doctors;

    for (const auto& row : csv.data) {
        Doctor d{};
        d.name = row[csv.header["Name"]];
        d.specialty = row[csv.header["Specialty"]];
        d.visitFee = std::stoi(row[csv.header["Visit Fee"]]);
        d.visitDuration = std::stoi(row[csv.header["Visit Duration"]]);
        d.waitTime = std::stoi(row[csv.header["Wait Time Average"]]);

        std::string available = row[csv.header["Days Available"]];
        auto parts = split(available, consts::DELIM_TIMES);
        for (const std::string& date : parts) {
            auto dateParts = split(date, consts::DELIM_DATE);
            Weekday wd = weekdayMap[dateParts[0]];
            Interval interval{};
            interval.start.day = wd;
            interval.start.hour = std::stoi(dateParts[1]);
            interval.end.day = wd;
            interval.end.hour = std::stoi(dateParts[2]);
            d.available.push_back(interval);
        }
        std::sort(d.available.begin(), d.available.end(), [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

        doctors.push_back(d);
    }

    return doctors;
}

std::unordered_map<std::string, std::string> initializeDiseases() {
    Csv csv = readCsv(consts::CSV_DISEASES);
    std::unordered_map<std::string, std::string> issueToSpecialty;

    for (const auto& row : csv.data) {
        std::string specialty = row[csv.header["Specialty"]];
        std::string diseases = row[csv.header["Diseases"]];
        auto issues = split(diseases, consts::DELIM_TIMES);
        for (const std::string& issue : issues) {
            issueToSpecialty[issue] = specialty;
        }
    }

    return issueToSpecialty;
}

Data initializeInput() {
    Data data;
    data.patients = initializePatients();
    data.doctors = initializeDoctors();
    data.issueToSpecialty = initializeDiseases();
    return data;
}

Time calcNextTime(Time first, int visitDuration) {
    first.hour += visitDuration / 60;
    first.minute += visitDuration % 60;
    if (first.minute >= 60) {
        first.hour += first.minute / 60;
        first.minute %= 60;
    }
    return first;
}

const Interval* findLastTaken(const Interval& available, const std::vector<Interval>& takenIntervals) {
    const Interval* lastTakenInInterval = nullptr;
    bool foundStart = false;
    for (const Interval& taken : takenIntervals) {
        if (taken.start >= available.start && taken.end <= available.end) {
            foundStart = true;
            lastTakenInInterval = &taken;
        }
        else if (foundStart) break;
    }
    return lastTakenInInterval;
}

Interval getFirstAvailableTime(const Doctor& doctor) {
    for (Interval available : doctor.available) {
        auto lastTaken = findLastTaken(available, doctor.taken);
        if (lastTaken == nullptr) {
            Time nextEndTime = calcNextTime(available.start, doctor.visitDuration);
            if (nextEndTime <= available.end) {
                return Interval{available.start, nextEndTime};
            }
            continue;
        }
        Time nextEndTime = calcNextTime(lastTaken->end, doctor.visitDuration);
        if (nextEndTime <= available.end) {
            return Interval{lastTaken->end, nextEndTime};
        }
    }
    return {Time{Weekday::Sat, -1, -1}, Time{Weekday::Sat, -1, -1}};
}

void schedule(Data& data) {
    std::sort(data.patients.begin(), data.patients.end(), [](const Patient& a, const Patient& b) {
        if (a.appointed < b.appointed) {
            return true;
        }
        if (a.appointed == b.appointed) {
            return a.name < b.name;
        }
        return false;
    });

    std::unordered_map<std::string, std::vector<Doctor*>> specialtyToDoctorsCache;
    for (Patient& p : data.patients) {
        std::vector<Doctor*> wantedDoctors;
        auto specialty = data.issueToSpecialty[p.medicalIssue];
        auto itr = specialtyToDoctorsCache.find(specialty);
        if (itr == specialtyToDoctorsCache.end()) {
            for (Doctor& d : data.doctors) {
                if (d.specialty == specialty) {
                    wantedDoctors.push_back(&d);
                }
            }
            specialtyToDoctorsCache[specialty] = wantedDoctors;
        }
        else {
            wantedDoctors = itr->second;
        }

        std::vector<std::pair<Interval, Doctor*>> choices;
        for (Doctor* d : wantedDoctors) {
            Interval first = getFirstAvailableTime(*d);
            if (first.start.hour == -1) continue;
            choices.push_back({first, d});
        }
        if (choices.empty()) {
            p.res.doc = nullptr;
            continue;
        }

        std::sort(choices.begin(), choices.end(), [](const auto a, const auto b) {
            if (a.first.start < b.first.start) return true;
            if (a.first.start == b.first.start) {
                if (a.second->waitTime < b.second->waitTime) return true;
                if (a.second->waitTime == b.second->waitTime) {
                    return a.second->name < b.second->name;
                }
            }
            return false;
        });
        Doctor* chosenDoctor = choices.front().second;
        chosenDoctor->taken.push_back(choices.front().first);
        p.res.doc = chosenDoctor;
        p.res.start = choices.front().first.start;
    }
}

int getVisitNum(const Patient& p) {
    int count = 0;
    for (const Interval i : p.res.doc->taken) {
        if (i.start.day != p.res.start.day) continue;
        ++count;
        if (i.start == p.res.start) return count;
    }
    return -1;
}

void printOutput(Data& data) {
    std::sort(data.patients.begin(), data.patients.end(), [](const Patient& a, const Patient& b) {
        return a.name < b.name;
    });

    std::cout.fill('0');
    for (const Patient& p : data.patients) {
        std::cout << "Name: " << p.name << '\n';
        if (p.res.doc == nullptr) {
            std::cout << "No free time\n";
        }
        else {
            std::string weekday = weekdayStringMap[p.res.start.day];
            std::cout << "Doctor: " << p.res.doc->name << '\n';
            std::cout << "Visit: " << weekday << ' ' << getVisitNum(p) << ' '
                      << std::setw(2) << p.res.start.hour << ':' << std::setw(2) << p.res.start.minute << '\n';
            std::cout << "Charge: " << p.res.doc->visitFee << '\n';
        }
        if (&p == &data.patients.back()) break;
        std::cout << "----------\n";
    }
}

int main() {
    Data data = initializeInput();
    schedule(data);
    printOutput(data);
    return 0;
}
