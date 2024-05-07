project(TeaEngine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${INCLUDE_DIR}/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(tea-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${INCLUDE_DIR}
)