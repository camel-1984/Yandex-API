#pragma once

#include "path.h"
#include "utils.h"
#include "api.h"

void SaveCityCache(const std::vector<Place>& settlements);

std::vector<Place> LoadCityCache();

std::vector<Place> GET_HDD_CACHE_CITY();

std::string GetDirectCacheKey(const std::string& from, const std::string& to, const std::string& date);

void SAVE_RAM_CACHE_DIRECT(const std::string& from, const std::string& to, const std::string& date, const nlohmann::json& response);

nlohmann::json GET_RAM_CACHE_DIRECT(const std::string& from, const std::string& to, const std::string& date);

struct Route;

std::string GetCacheKey(const std::string& from, const std::string& to, const std::string& date);

void SAVE_RAM_CACHE_ROUTE(const std::string& from, const std::string& to, const std::string& date, const std::vector<Route>& routes);

std::vector<Route> GET_RAM_CACHE_ROUTE(const std::string& from, const std::string& to, const std::string& date);












