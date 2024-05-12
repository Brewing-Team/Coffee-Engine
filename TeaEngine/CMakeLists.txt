project(TeaEngine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(tea-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    spdlog::spdlog
    fmt::fmt)