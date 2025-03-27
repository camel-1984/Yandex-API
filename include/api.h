#pragma once

#include "utils.h"

struct Place {
    std::string code;
    std::string title;
};

cpr::Response API_GET_DIRECT(const Data& data, const std::string& from, const std::string& to);

std::vector<Place> API_GET_SETTLEMENTS();

void SortSettlements(std::vector<Place>& vec);

// void PrintCodes(const std::vector<Place>& vec);




