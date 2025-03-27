#include "include/api.h"

cpr::Response API_GET_DIRECT(const Data& data, const std::string& from, const std::string& to) {

    std::string api_key = EnvLoader::Get();
    std::string date = data.GetDate();

    cpr::Response response = cpr::Get (
        cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
        cpr::Parameters {
            {"format", "json"},
            {"lang", "ru_RU"},
            {"apikey", api_key},

            {"date", date}, //YYYY-MM-DD
            {"from", from},
            {"to", to},
        }
    );

    return response;
}

std::vector<Place> API_GET_SETTLEMENTS() {

    std::string api_key = EnvLoader::Get();

    cpr::Response response = cpr::Get(
        cpr::Url{"https://api.rasp.yandex.net/v3.0/stations_list/"},
        cpr::Parameters{
            {"apikey", api_key},
            {"lang", "ru_RU"},
            {"format", "json"}
        }
    );

    if (response.status_code != 200) {
        std::cerr << response.status_code << '\n';
        throw std::runtime_error("Ошибка API: \n");
    }

    nlohmann::json codes = nlohmann::json::parse(response.text);

    std::vector<Place> vec;

    if (codes.contains("countries")) {
        for (const auto& country : codes["countries"]) {
            if (country.contains("regions")) {
                for (const auto& region : country["regions"]) {
                    if (region.contains("settlements")) {
                        for (const auto& settlement : region["settlements"]) {
                            Place place;
                            place.code = settlement["codes"].value("yandex_code", "Неизвестно");
                            place.title = settlement.value("title", "Неизвестно");
                            if (place.code != "Неизвестно" && place.title != "Неизвестно") {
                                vec.push_back(place);
                            }
                        }
                    }
                }
            }
        }
    }

    return vec;
}

void SortSettlements(std::vector<Place>& vec) {

    std::vector<std::string> allowed_cities = {
        "Санкт-Петербург",
        "Москва",
        "Калининград",
        "Самара",
        "Саратов",
        "Минск",
        "Казань",
        "Ростов-на-Дону",
        "Краснодар",
        "Воронеж",
        "Челябинск",
        "Нижний Новгород",
        "Волгоград",
        "Мурманск",
        "Калуга",
        "Брянск",
        "Владимир",
        "Курск",
        "Тула"
    };
    std::vector<Place> filtered_vec;

    for (const Place& place : vec) {
        for (const std::string& city : allowed_cities) {
            if (place.title == city) {
                filtered_vec.push_back(place);
                break;
            }
        }
    }

    vec = filtered_vec;
}

// void PrintCodes(const std::vector<Place>& vec) {
//     std::cout << vec.size() << '\n';
//     for (const auto& i : vec) {
//         std::cout << i.title << '\n';
//         std::cout << i.code << '\n';
//         std::cout << "--------" << '\n';
//     }
// }

// cpr::Response SPB_KLG(const Data& data) {
//
//     std::string api_key = EnvLoader::Get();
//     std::string from = "c2";  // с2 - Санкт-Петербург
//     std::string to = "c22"; // с213 - Москва с22 - Калининград
//     std::string date = data.GetDate();
//
//     cpr::Response response = cpr::Get (
//         cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
//         cpr::Parameters {
//             {"format", "json"},
//             {"lang", "ru_RU"}, //uk_UA
//             {"apikey", api_key},
//
//             {"date", date}, //YYYY-MM-DD
//             {"from", from},
//             {"to", to},
//             // {"transfers", "true"}
//         }
//     );
//     return response;
// }
//
// cpr::Response KLG_SPB(const Data& data) {
//
//     std::string api_key = EnvLoader::Get();
//     std::string from = "c22";  // с2 - Санкт-Петербург
//     std::string to = "c2"; // с213 - Москва с22 - Калининград
//     std::string date = data.GetDate();
//
//     cpr::Response response = cpr::Get (
//         cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
//         cpr::Parameters {
//             {"format", "json"},
//             {"lang", "ru_RU"}, //uk_UA
//             {"apikey", api_key},
//
//             {"date", date}, //YYYY-MM-DD
//             {"from", from},
//             {"to", to},
//             // {"transfers", "true"}
//         }
//     );
//     return response;
// }

// void ParseResponse(const nlohmann::json& target) {
//     if (target.contains("pagination")) {
//         int total = target["pagination"].value("total", 0);
//         std::cout << "total routes: " << total << "\n";
//         std::cout << "--------------------------------------\n";
//     } else {
//         std::cout << "Нет данных о маршрутах.\n";
//     }
//
//     if (target.contains("segments")) {
//         for (const auto& i : target["segments"]) {
//             std::string departure_time = i.value("departure", "Неизвестно");
//             std::string arrival_time = i.value("arrival", "Неизвестно");
//
//             std::string departure_station_name = "Неизвестно";
//             if (i.contains("from") && i["from"].contains("title") && i["from"]["title"].is_string()) {
//                 departure_station_name = i["from"]["title"];
//             }
//
//             std::string departure_transport_type = "Неизвестно";
//             if (i.contains("from") && i["from"].contains("transport_type") && i["from"]["transport_type"].is_string()) {
//                 departure_transport_type = i["from"]["transport_type"];
//             }
//
//             std::string arrival_station_name = "Неизвестно";
//             if (i.contains("to") && i["to"].contains("title") && i["to"]["title"].is_string()) {
//                 arrival_station_name = i["to"]["title"];
//             }
//
//             std::string arrival_transport_type = "Неизвестно";
//             if (i.contains("to") && i["to"].contains("transport_type") && i["to"]["transport_type"].is_string()) {
//                 arrival_transport_type = i["to"]["transport_type"];
//             }
//
//             std::string stops = "Нет остановок";
//             if (i.value("stops", "Нет остановок") != "") {
//                 stops = i.value("stops", "Нет остановок");
//
//             }
//
//             bool has_transfers = i.value("has_transfers", false);
//
//             std::cout << "departure time: " << departure_time << "\n";
//             std::cout << "departure station: " << departure_station_name << '\n';
//             std::cout << "departure transport: " << departure_transport_type << '\n';
//             std::cout << "--------------------------------------\n";
//             std::cout << "arrival time: " << arrival_time << "\n";
//             std::cout << "arrival station: " << arrival_station_name << '\n';
//             std::cout << "arrival transport: " << arrival_transport_type << '\n';
//             std::cout << "--------------------------------------\n";
//             std::cout << "stops: " << stops << '\n';
//             std::cout << "--------------------------------------\n";
//         }
//     } else {
//         std::cout << "Нет данных о маршрутах.\n";
//     }
// }
