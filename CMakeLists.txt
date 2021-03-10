cmake_minimum_required(VERSION 3.4.1)

project(spp)

add_executable(spp src/main.cpp)

file(GLOB SRC_LIST "*.cpp" "src/*.cpp")
file(GLOB SRC_LIST "*.hpp" "src/*.hpp")
file(GLOB SRC_LIST "*.hpp" "3rd/*.hpp")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/asio
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/rapidjson
)

target_link_libraries(demo libface.so) 