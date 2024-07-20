project(TeaEditor VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PANEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Panels")


file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PANEL_DIR}/*.cpp")

SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    tea-engine)