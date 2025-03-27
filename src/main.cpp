#include <include/api.h>
#include <include/cache.h>

int main(int argc, char** argv) {

    EnvLoader::LoadEnv();
    std::string from_code = "c2";
    std::string to_code = "c22";

    Data data(ParseConsole(argc, argv));

    std::vector<Place> settlements = GET_HDD_CACHE_CITY();

    while (true) {

        for (const Place& i : settlements) {
            if (i.title == data.GetDestFrom()){
                from_code = i.code;
            }
        }

        for (const Place& i : settlements) {
            if (i.title == data.GetDestTo()){
                to_code = i.code;
            }
        }

        nlohmann::json spb = GET_RAM_CACHE_DIRECT(from_code , to_code, data.GetDate());
        nlohmann::json klg = GET_RAM_CACHE_DIRECT(to_code, from_code , data.GetDate());

        if (spb.empty()) {
            cpr::Response RES_SPB_KLG = API_GET_DIRECT(data, from_code , to_code);
            if (RES_SPB_KLG.status_code == 200) {
                spb = nlohmann::json::parse(RES_SPB_KLG.text);
                SAVE_RAM_CACHE_DIRECT(from_code , to_code, data.GetDate(), spb);
            } else {
                std::cerr << "Ошибка: " << RES_SPB_KLG.status_code << '\n';
                throw std::runtime_error("Error: bad status!\n");
            }
        }

        if (klg.empty()) {
            cpr::Response RES_KLG_SPB = API_GET_DIRECT(data, to_code, from_code );
            if (RES_KLG_SPB.status_code == 200) {
                klg = nlohmann::json::parse(RES_KLG_SPB.text);
                SAVE_RAM_CACHE_DIRECT(to_code, from_code , data.GetDate(), klg);
            } else {
                std::cerr << "Ошибка: " << RES_KLG_SPB.status_code << '\n';
                throw std::runtime_error("Error: bad status!\n");
            }
        }

        std::vector<Trip> vec = BuildTrip(spb, klg);
        PrintTrip(vec);


        std::vector<Route> routes_toward = GET_RAM_CACHE_ROUTE(from_code , to_code, data.GetDate());
        std::vector<Route> routes_back = GET_RAM_CACHE_ROUTE(to_code, from_code , data.GetDate());

        if (routes_toward.empty()) {
            routes_toward = BuildRoute_SPB_KLG(data, from_code , to_code);
            SAVE_RAM_CACHE_ROUTE(from_code , to_code, data.GetDate(), routes_toward);
        }

        if (routes_back.empty()) {
            routes_back = BuildRoute_KLG_SPB(data, to_code, from_code );
            SAVE_RAM_CACHE_ROUTE(to_code, from_code , data.GetDate(), routes_back);
        }

        std::vector<RoundTrip> roundTrips = BuildRoundTrip(routes_toward, routes_back);
        PrintRoundTrips(roundTrips);

        std::cout << "Введите дату и/или + два города или 'exit' для завершения: ";

        std::string input_line;
        std::getline(std::cin, input_line);

        std::istringstream iss(input_line);
        std::string new_date, new_from, new_to;

        iss >> new_date;

        if (new_date == "exit") {
            return 0;
        }
        if (iss >> new_from >> new_to) {
            data = Data(new_date, new_from, new_to);
        } else {
            data = Data(new_date);
        }
        std::cout << "\n";
    }

    return 0;
}
