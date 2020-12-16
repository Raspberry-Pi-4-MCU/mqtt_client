project(mqtt)
cmake_minimum_required(VERSION 3.2)
add_definitions(-std=c++14 -Wall -g)
find_package(nlohmann_json REQUIRED)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
#
add_executable(main src/main.cpp src/my_mqtt.cpp)
target_link_libraries(main -lpaho-mqtt3as -lpaho-mqttpp3 -lcurlpp -lcurl)
