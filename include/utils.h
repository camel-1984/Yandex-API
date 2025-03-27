#pragma once

#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "env.h"

struct Data {
private:
    std::string date_;
    std::string destination_to_;
    std::string destination_from_;

public:
    explicit Data(const std::string& date) {
        date_ = date;
    }

    Data(const std::string& date, const std::string& destination_from, const std::string& destination_to) {
        date_ = date;
        destination_from_ = destination_from;
        destination_to_ = destination_to;

    }

    // Data(const std::string& date, const std::string& destination) {
    //     date_ = date;
    //     destination_ = destination;
    // }

    std::string GetDate() const {
        return date_;
    }

    std::string GetDestTo() const {
        return destination_to_;
    }

    std::string GetDestFrom() const {
        return destination_from_;
    }

    // void ChangeDate(const std::string& str) {
    //     date_ = str;
    // }

    // std::string GetDest() const {
    //     return destination_;
    // }
};

Data ParseConsole(int argc, char** argv);

void CheckDate(const std::string& date);

uint64_t ToSec(const std::string& time);

// void CheckDest(const std::string& dest);