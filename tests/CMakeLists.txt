project(${PROJECT_NAME}_test)

enable_testing()

include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG origin/main
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "../source/*.cpp")
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../source/main.cpp")
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main gmock)
link_common_libraries(${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
target_link_options(${PROJECT_NAME} PRIVATE --coverage)