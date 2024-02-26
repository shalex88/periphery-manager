cmake_minimum_required(VERSION 3.22)
include(cmake/SetVersion.cmake)
project(periphery-manager LANGUAGES CXX VERSION ${VERSION})

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog fmt::fmt)

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(cmake/CreatePackage.cmake)
