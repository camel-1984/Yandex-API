#include "include/utils.h"

Data ParseConsole(int argc, char** argv) {

    if (argc == 2) {
        CheckDate(argv[1]);
        return Data{argv[1]};
    }

    if (argc == 4) {
        CheckDate(argv[1]);
        return Data{argv[1], argv[2], argv[3]};
    }

    // if (argc == 3) {
    //     CheckDest(argv[2]);
    //     return Data{argv[1], argv[2]};
    // }

    throw std::runtime_error("Error: wrong args!\n");
}

void CheckDate(const std::string& date) {

    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        throw std::runtime_error("Error: wrong date! Need: YYYY-MM-DD\n");
    }

    for (size_t i = 0; i < date.length(); ++i) {
        if ((i != 4 && i != 7) && !isdigit(date[i])) {
            throw std::runtime_error("Error: wrong date! Need: YYYY-MM-DD\n");
        }
    }

    int year, month, day;
    char dash1, dash2;
    std::istringstream iss(date);
    if (!(iss >> year >> dash1 >> month >> dash2 >> day)) {
        throw std::runtime_error("Error: wrong date! Need: YYYY-MM-DD\n");
    }
    if (year < 2025 || year > 2026) {
        throw std::runtime_error("Error: wrong date! Need: YYYY - [2025;2026]\n");
    }
    if (month < 1 || month > 12) {
        throw std::runtime_error("Error: wrong date! Need: MM - [01;12]\n");
    }
    if (day < 1 || day > 31) {
        throw std::runtime_error("Error: wrong date! Need: DD - [01;31\n");
    }
}

uint64_t ToSec(const std::string& time) {

    uint64_t hour = std::stoull(time.substr(11,2));
    uint64_t min = std::stoull(time.substr(14,2));
    uint64_t sec = std::stoull(time.substr(17,2));

    uint64_t result = hour * 3600 + min * 60 + sec;

    return result;
}

// void CheckDest(const std::string& dest) {
//     if (dest.empty()) {
//         throw std::runtime_error("Error: wrong destination!\n");
//     }
//     if (dest[0] != 'c') {
//         throw std::runtime_error("Error: wrong destination!\n");
//     }
//     for (size_t i = 1; i < dest.length(); i++) {
//         if (!isdigit(dest[i])) {
//             throw std::runtime_error("Error: wrong destination!\n");
//         }
//     }
// }