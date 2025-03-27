add_executable(src api.cpp cache.cpp main.cpp path.cpp utils.cpp)

target_include_directories(src PUBLIC ${PROJECT_SOURCE_DIR})