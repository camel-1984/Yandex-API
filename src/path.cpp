#include  "include/path.h"


std::vector<Trip> BuildTrip(const nlohmann::json& SPB, const nlohmann::json& KLG) {

    std::vector<Trip> vec;

    if (SPB.contains("segments") && KLG.contains("segments")) {
        for (const auto& i : SPB["segments"]) {

            Trip final;
            final.time_departure_SPB_KLG = i.value("departure", "Неизвестно");
            final.time_arrival_SPB_KLG = i.value("arrival", "Неизвестно");

            final.station_departure_SPB_KLG = i["from"].value("title", "Неизвестно");
            final.station_arrival_SPB_KLG = i["to"].value("title", "Неизвестно");

            final.transport_SPB_KLG = i["from"].value("transport_type","Неизвестно");

            uint64_t time_arrival_SPB_KLG = ToSec(final.time_arrival_SPB_KLG);

            for (const auto& j : KLG["segments"]) {

                final.time_departure_KLG_SPB = j.value("departure", "Неизвестно");
                final.time_arrival_KLG_SPB = j.value("arrival", "Неизвестно");

                final.station_departure_KLG_SPB = j["from"].value("title", "Неизвестно");
                final.station_arrival_KLG_SPB = j["to"].value("title", "Неизвестно");

                final.transport_KLG_SPB = j["from"].value("transport_type","Неизвестно");

                uint64_t time_departure_KLG_SPB = ToSec(final.time_departure_KLG_SPB);

                uint64_t transfer_time = time_departure_KLG_SPB - time_arrival_SPB_KLG;

                if (transfer_time >= 3600 && transfer_time <= 10800) {
                    vec.push_back(final);
                }
            }
        }
    } else {
        std::cout << "Нет данных о маршрутах.\n";
    }
    return vec;
}

void PrintTrip(const std::vector<Trip>& vec) {
    std::cout << "Найдено маршрутов без пересадки: " << vec.size() << "\n\n";

    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << " Маршрут № " << (i + 1) << "\n";
        std::cout << "----------------------------\n";
        std::cout << " Отправление: " << vec[i].station_departure_SPB_KLG << " → " << vec[i].station_arrival_SPB_KLG << "\n";
        std::cout << " Время: " << vec[i].time_departure_SPB_KLG << " → " << vec[i].time_arrival_SPB_KLG << "\n";
        std::cout << " Вид транспорта: " << vec[i].transport_SPB_KLG << "\n";
        std::cout << "----------------------------\n";
        std::cout << " Возвращение: " << vec[i].station_departure_KLG_SPB << " → " << vec[i].station_arrival_KLG_SPB << "\n";
        std::cout << " Время: " << vec[i].time_departure_KLG_SPB << " → " << vec[i].time_arrival_KLG_SPB << "\n";
        std::cout << " Вид транспорта: " << vec[i].transport_KLG_SPB << "\n";
        std::cout << "============================\n\n";
        if (i == 9) {
            break;
        }
    }
}

std::vector<Route> BuildRoute_KLG_SPB(const Data& data, const std::string& klg, const std::string& spb) {
    std::vector<Route> routes;
    std::vector<Place> settlements = GET_HDD_CACHE_CITY();

    std::string api_key = EnvLoader::Get();
    std::string date = data.GetDate();

    for (const auto& settlement : settlements) {
        std::string else_city = settlement.code;
        if (else_city == data.GetDestFrom() || else_city == data.GetDestTo()) {
            continue;
        }

        cpr::Response resp1 = cpr::Get(
            cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
            cpr::Parameters{
                {"format", "json"},
                {"lang", "ru_RU"},
                {"apikey", api_key},
                {"date", date},
                {"from", klg},
                {"to", else_city}
            }
        );

        if (resp1.status_code != 200) {
            continue;
        }
        nlohmann::json json1 = nlohmann::json::parse(resp1.text);
        if (!json1.contains("segments"))
            continue;

        cpr::Response resp2 = cpr::Get(
                cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
                cpr::Parameters{
                    {"format", "json"},
                    {"lang", "ru_RU"},
                    {"apikey", api_key},
                    {"date", date},
                    {"from", else_city},
                    {"to", spb}
                }
            );
        if (resp2.status_code != 200) {
            continue;
        }
        nlohmann::json json2 = nlohmann::json::parse(resp2.text);
        if (!json2.contains("segments"))
            continue;

        for (const nlohmann::basic_json<>& i : json1["segments"]) {
            Route route;
            route.time_departure_CITY_ELSE = i.value("departure", "Неизвестно");
            route.time_arrival_CITY_ELSE = i.value("arrival", "Неизвестно");
            route.station_departure_CITY_ELSE = i["from"].value("title", "Неизвестно");
            route.station_arrival_CITY_ELSE = i["to"].value("title", "Неизвестно");
            route.transport_CITY_ELSE = i["from"].value("transport_type","Неизвестно");

            uint64_t arrival_time = ToSec(route.time_arrival_CITY_ELSE);

            for (const nlohmann::basic_json<>& j : json2["segments"]) {
                route.time_departure_ELSE_CITY = j.value("departure", "Неизвестно");
                route.time_arrival_ELSE_CITY = j.value("arrival", "Неизвестно");
                route.station_departure_ELSE_CITY = j["from"].value("title", "Неизвестно");
                route.station_arrival_ELSE_CITY = j["to"].value("title", "Неизвестно");
                route.transport_ELSE_CITY = j["from"].value("transport_type","Неизвестно");

                uint64_t departure_time = ToSec(route.time_departure_ELSE_CITY);
                uint64_t transfer_time = departure_time - arrival_time;

                if (transfer_time >= 3600 && transfer_time <= 10800) {
                    routes.push_back(route);

                }
            }
        }
    }
    return routes;
}

std::vector<Route> BuildRoute_SPB_KLG(const Data& data, const std::string& spb, const std::string& klg) {
    std::vector<Route> routes;
    std::vector<Place> settlements = GET_HDD_CACHE_CITY();

    std::string api_key = EnvLoader::Get();
    std::string date = data.GetDate();

    for (const Place& settlement : settlements) {
        std::string else_city = settlement.code;
        if (else_city == data.GetDestFrom() || else_city == data.GetDestTo()) {
            continue;
        }

        cpr::Response resp1 = cpr::Get(
            cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
            cpr::Parameters{
                {"format", "json"},
                {"lang", "ru_RU"},
                {"apikey", api_key},
                {"date", date},
                {"from", spb},
                {"to", else_city}
            }
        );

        if (resp1.status_code != 200) {
            continue;
        }
        nlohmann::json json1 = nlohmann::json::parse(resp1.text);
        if (!json1.contains("segments"))
            continue;

        cpr::Response resp2 = cpr::Get(
                cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"},
                cpr::Parameters{
                    {"format", "json"},
                    {"lang", "ru_RU"},
                    {"apikey", api_key},
                    {"date", date},
                    {"from", else_city},
                    {"to", klg}
                }
            );
        if (resp2.status_code != 200) {
            continue;
        }
        nlohmann::json json2 = nlohmann::json::parse(resp2.text);
        if (!json2.contains("segments"))
            continue;

        for (const nlohmann::basic_json<>& i : json1["segments"]) {
            Route route;
            route.time_departure_CITY_ELSE = i.value("departure", "Неизвестно");
            route.time_arrival_CITY_ELSE = i.value("arrival", "Неизвестно");
            route.station_departure_CITY_ELSE = i["from"].value("title", "Неизвестно");
            route.station_arrival_CITY_ELSE = i["to"].value("title", "Неизвестно");
            route.transport_CITY_ELSE = i["from"].value("transport_type","Неизвестно");

            uint64_t arrival_time = ToSec(route.time_arrival_CITY_ELSE);

            for (const nlohmann::basic_json<>& j : json2["segments"]) {
                route.time_departure_ELSE_CITY = j.value("departure", "Неизвестно");
                route.time_arrival_ELSE_CITY = j.value("arrival", "Неизвестно");
                route.station_departure_ELSE_CITY = j["from"].value("title", "Неизвестно");
                route.station_arrival_ELSE_CITY = j["to"].value("title", "Неизвестно");
                route.transport_ELSE_CITY = j["from"].value("transport_type","Неизвестно");

                uint64_t departure_time = ToSec(route.time_departure_ELSE_CITY);
                uint64_t transfer_time = departure_time - arrival_time;

                if (transfer_time >= 3600 && transfer_time <= 10800) {
                    routes.push_back(route);
                }
            }
        }
    }
    return routes;
}


std::vector<RoundTrip> BuildRoundTrip(const std::vector<Route>& goingRoutes, const std::vector<Route>& backRoutes) {

    std::vector<RoundTrip> roundTrips;

    uint64_t minLayover = 1 * 3600;
    uint64_t maxLayover = 3 * 3600;

    for (const Route& i : goingRoutes) {

        uint64_t arrivalKlg = ToSec(i.time_arrival_ELSE_CITY);

        for (const Route& j : backRoutes) {
            uint64_t departureKlg = ToSec(j.time_departure_CITY_ELSE);

            if (departureKlg > arrivalKlg) {
                uint64_t layover = departureKlg - arrivalKlg;
                if (layover >= minLayover && layover <= maxLayover) {
                    RoundTrip rt;
                    rt.going = i;
                    rt.back  = j;
                    roundTrips.push_back(rt);

                }
            }
        }
    }
    return roundTrips;
}

void PrintRoundTrips(const std::vector<RoundTrip>& roundTrips) {
    if (roundTrips.empty()) {
        std::cout << "Нет найденных маршрутов.\n";
        return;
    }

    std::cout << "Найдено маршрутов с пересадкой: " << roundTrips.size() << "\n\n";

    for (size_t i = 0; i < roundTrips.size(); i++) {
        const RoundTrip& rt = roundTrips[i];
        std::cout << " Маршрут номер № " << (i + 1) << ":\n\n";

        std::cout << "Туда:\n";
        std::cout << "    Отправление: " << rt.going.station_departure_CITY_ELSE
                  << " в " << rt.going.time_departure_CITY_ELSE << "\n";
        std::cout << "    Прибытие: " << rt.going.station_arrival_CITY_ELSE
                  << " в " << rt.going.time_arrival_CITY_ELSE << "\n";
        std::cout << "    Вид транспорта: " << rt.going.transport_CITY_ELSE << "\n";

        uint64_t arrivalGoing = ToSec(rt.going.time_arrival_CITY_ELSE);
        uint64_t departureGoing = ToSec(rt.going.time_departure_ELSE_CITY);
        uint64_t layoverGoing = departureGoing - arrivalGoing;
        std::cout << "    Время пересадки: "
                  << layoverGoing / 3600 << " ч " << (layoverGoing % 3600) / 60 << " мин\n";

        std::cout << "    Отправление: " << rt.going.station_departure_ELSE_CITY
                  << " в " << rt.going.time_departure_ELSE_CITY << "\n";
        std::cout << "    Прибытие: " << rt.going.station_arrival_ELSE_CITY
                  << " в " << rt.going.time_arrival_ELSE_CITY << "\n";
        std::cout << "    Вид транспорта: " << rt.going.transport_ELSE_CITY << "\n\n";

        std::cout << "Обратно:\n";
        std::cout << "    Отправление: " << rt.back.station_departure_CITY_ELSE
                  << " в " << rt.back.time_departure_CITY_ELSE << "\n";
        std::cout << "    Прибытие: " << rt.back.station_arrival_CITY_ELSE
                  << " в " << rt.back.time_arrival_CITY_ELSE << "\n";
        std::cout << "    Вид транспорта: " << rt.back.transport_CITY_ELSE << "\n";


        uint64_t arrivalBack = ToSec(rt.back.time_arrival_CITY_ELSE);
        uint64_t departureBack = ToSec(rt.back.time_departure_ELSE_CITY);
        uint64_t layoverBack = departureBack - arrivalBack;
        std::cout << "    Время пересадки: "
                  << layoverBack / 3600 << " ч " << (layoverBack % 3600) / 60 << " мин\n";

        std::cout << "    Отправление: " << rt.back.station_departure_ELSE_CITY
                  << " в " << rt.back.time_departure_ELSE_CITY << "\n";
        std::cout << "    Прибытие: " << rt.back.station_arrival_ELSE_CITY
                  << " в " << rt.back.time_arrival_ELSE_CITY << "\n";
        std::cout << "    Вид транспорта: " << rt.back.transport_ELSE_CITY << "\n";

        std::cout << "-------------------------------------------\n\n";
        if(i==9) {
            break;
        }
    }
}

// void PrintRoutes(const std::vector<Route>& obj1, const std::vector<Route>& obj2) {
//     if (obj1.empty() || obj2.empty()) {
//         std::cout << " Нет найденных маршрутов с пересадками.\n";
//         return;
//     }
//
//     std::cout << " Найдено маршрутов: " << routes.size() << "\n\n";
//
//     size_t index = 1;
//     for (const auto& route : routes) {
//         std::cout << "============================\n";
//         std::cout << " Маршрут №" << index++ << "\n";
//         std::cout << "----------------------------\n";
//
//         std::cout << " Отправление: " << route.station_departure_CITY_ELSE << " → " << route.station_arrival_CITY_ELSE << "\n";
//         std::cout << " Время: " << route.time_departure_CITY_ELSE << " → " << route.time_arrival_CITY_ELSE << "\n";
//         std::cout << "----------------------------\n";
//
//         std::cout << " Пересадка: " << route.station_departure_ELSE_CITY << " → " << route.station_arrival_ELSE_CITY << "\n";
//         std::cout << " Время: " << route.time_departure_ELSE_CITY << " → " << route.time_arrival_ELSE_CITY << "\n";
//         std::cout << "----------------------------\n";
//
//         std::cout << " Возвращение: " << route.station_departure_CITY_ELSE << " → " << route.station_arrival_CITY_ELSE << "\n";
//         std::cout << " Время: " << route.time_departure_CITY_ELSE << " → " << route.time_arrival_CITY_ELSE << "\n";
//         std::cout << "----------------------------\n";
//
//         std::cout << " Пересадка: " << route.station_departure_ELSE_CITY << " → " << route.station_arrival_ELSE_CITY << "\n";
//         std::cout << " Время: " << route.time_departure_ELSE_CITY << " → " << route.time_arrival_ELSE_CITY << "\n";
//         std::cout << "============================\n\n";
//         if (index==10) {
//             break;
//         }
//     }
// }



