include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
include(cmake/SetVersion.cmake)
project(periphery-manager LANGUAGES CXX VERSION ${VERSION})

find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include source)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

function(link_common_libraries target)
    target_link_libraries(${target}
            PRIVATE
            #Add your libs here
            spdlog::spdlog
            asio
            asio::asio
    )
endfunction(link_common_libraries)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
link_common_libraries(${PROJECT_NAME})

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(cmake/CreatePackage.cmake)
