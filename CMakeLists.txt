include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
include(cmake/SetVersion.cmake)
project(periphery-manager LANGUAGES CXX VERSION ${VERSION})

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

find_package(spdlog CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

# Set variables for protobuf usage
set(PROTO_DIR "proto")
set(GENERATED_DIR "generated")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${GENERATED_DIR})
find_package(absl REQUIRED)
find_package(Protobuf REQUIRED)
file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS "${PROTO_DIR}/*.proto")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR} ${CMAKE_BINARY_DIR}/${GENERATED_DIR} ${Protobuf_INCLUDE_DIRS})
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROTO_SOURCE_FILES})

function(link_common_libraries target)
    target_link_libraries(${target}
            PRIVATE
            #Add your libs here
            spdlog::spdlog
            absl::base
            protobuf::libprotoc
            protobuf::libprotobuf
    )
endfunction(link_common_libraries)

link_common_libraries(${PROJECT_NAME})

protobuf_generate(
        TARGET ${PROJECT_NAME}
        PROTOS ${PROTO_FILES}
        OUT_VAR PROTO_SOURCE_FILES
        PROTOC_OUT_DIR ${CMAKE_BINARY_DIR}/${GENERATED_DIR}
)

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
include(cmake/CreatePackage.cmake)
