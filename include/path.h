#pragma once

#include "utils.h"
#include "cache.h"

struct Trip {

    std::string time_departure_SPB_KLG;
    std::string time_arrival_SPB_KLG;
    std::string time_departure_KLG_SPB;
    std::string time_arrival_KLG_SPB;

    std::string transport_SPB_KLG;
    std::string transport_KLG_SPB;

    std::string station_departure_SPB_KLG;
    std::string station_arrival_SPB_KLG;
    std::string station_departure_KLG_SPB;
    std::string station_arrival_KLG_SPB;

};

struct Route {

    std::string time_departure_CITY_ELSE;
    std::string station_departure_CITY_ELSE;

    std::string transport_CITY_ELSE;

    std::string time_arrival_CITY_ELSE;
    std::string station_arrival_CITY_ELSE;

    std::string time_departure_ELSE_CITY;
    std::string station_departure_ELSE_CITY;

    std::string transport_ELSE_CITY;

    std::string time_arrival_ELSE_CITY;
    std::string station_arrival_ELSE_CITY;

};

struct RoundTrip {
    Route going;
    Route back;
};

std::vector<RoundTrip> BuildRoundTrip(const std::vector<Route>& goingRoutes, const std::vector<Route>& backRoutes);

std::vector<Trip> BuildTrip(const nlohmann::json& obj1, const nlohmann::json& obj2);

void PrintTrip(const std::vector<Trip>& vec);

void PrintRoundTrips(const std::vector<RoundTrip>& roundTrips);

std::vector<Route> BuildRoute_KLG_SPB(const Data& data, const std::string& klg, const std::string& spb);

std::vector<Route> BuildRoute_SPB_KLG(const Data& data, const std::string& spb, const std::string& klg);

// void PrintRoutes(const std::vector<Route>& routes);




