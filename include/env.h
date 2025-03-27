#pragma once
#include <fstream>
#include <iostream>

class EnvLoader {
public:
    static void LoadEnv(const std::string& filename = "/Users/rostislavmolcanov/Documents/itmo/op/sem2/lab02/lab6/labwork6-camel-1984/.env") {
        std::ifstream file(filename);
        std::cout << "Загрузка...\n";
        if (!file) {
            throw std::runtime_error("Error: API-ключ не найден!\n");
        }

        std::string line;
        std::getline(file, line);
        key = line.substr(8,36);
    }

    static std::string Get() {
        return key;
    }

private:
    inline static std::string key;
};

