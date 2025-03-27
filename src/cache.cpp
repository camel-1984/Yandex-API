#include "include/cache.h"

const std::string FILE_HDD_CACHE_CITY = "city_cache.json";

void SaveCityCache(const std::vector<Place>& settlements) {
    std::ofstream file(FILE_HDD_CACHE_CITY);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << FILE_HDD_CACHE_CITY << "\n";
        return;
    }

    nlohmann::json json_data = nlohmann::json::array();
    for (const Place& place : settlements) {
        json_data.push_back({{"title", place.title}, {"code", place.code}});
    }

    file << json_data;
}

std::vector<Place> LoadCityCache() {
    std::vector<Place> settlements;

    if (!std::filesystem::exists(FILE_HDD_CACHE_CITY)) {
        return settlements;
    }

    std::ifstream file(FILE_HDD_CACHE_CITY);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << FILE_HDD_CACHE_CITY << "\n";
        return settlements;
    }

    nlohmann::json json_data;
    file >> json_data;

    for (const nlohmann::basic_json<>& city : json_data) {
        Place place;
        place.title = city.value("title", "Неизвестно");
        place.code = city.value("code", "Неизвестно");
        settlements.push_back(place);
    }
    SortSettlements(settlements);
    return settlements;
}

std::vector<Place> GET_HDD_CACHE_CITY() {
    std::vector<Place> result = LoadCityCache();
    if (!result.empty()) {
        return result;
    }

    result = API_GET_SETTLEMENTS();
    SaveCityCache(result);
    SortSettlements(result);
    return result;
}

std::unordered_map<std::string, nlohmann::json> RAM_CACHE_DIRECT;

std::string GetDirectCacheKey(const std::string& from, const std::string& to, const std::string& date) {
    return "direct_from=" + from + "_to=" + to + "_date=" + date;
}

void SAVE_RAM_CACHE_DIRECT(const std::string& from, const std::string& to, const std::string& date, const nlohmann::json& response) {
    std::string key = GetDirectCacheKey(from, to, date);
    RAM_CACHE_DIRECT[key] = response;
    // std::cout << " Сохранено в RAM-кэш (Direct): " << from << " → " << to << " на " << date << "\n";
}

nlohmann::json GET_RAM_CACHE_DIRECT(const std::string& from, const std::string& to, const std::string& date) {
    std::string key = GetDirectCacheKey(from, to, date);

    if (RAM_CACHE_DIRECT.find(key) != RAM_CACHE_DIRECT.end()) {
        // std::cout << " Direct-маршрут " << from << " → " << to << " на " << date << " загружен из RAM-кэша.\n";
        return RAM_CACHE_DIRECT[key];
    }

    return {};
}

std::unordered_map<std::string, std::vector<Route>> RAM_CACHE_ROUTES;

std::string GetCacheKey(const std::string& from, const std::string& to, const std::string& date) {
    return "from=" + from + "_to=" + to + "_date=" + date;
}

void SAVE_RAM_CACHE_ROUTE(const std::string& from, const std::string& to, const std::string& date, const std::vector<Route>& routes) {
    std::string key = GetCacheKey(from, to, date);
    RAM_CACHE_ROUTES[key] = routes;
    // std::cout << " Сохранено в RAM-кэш: " << from << " → " << to << " на " << date << "\n";
}

std::vector<Route> GET_RAM_CACHE_ROUTE(const std::string& from, const std::string& to, const std::string& date) {
    std::string key = GetCacheKey(from, to, date);

    if (RAM_CACHE_ROUTES.contains(key)) {
        // std::cout << " Маршрут " << from << " → " << to << " на " << date << " загружен из RAM-кэша.\n";
        return RAM_CACHE_ROUTES[key];
    }

    return {};
}
