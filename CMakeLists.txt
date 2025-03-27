cmake_minimum_required(VERSION 3.29)

project(cpp-Yandex-API)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.9.9)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(src)

target_link_libraries(src PUBLIC cpr::cpr nlohmann_json::nlohmann_json)


