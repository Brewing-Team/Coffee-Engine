project(TeaEngine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(glfw3 REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(tea-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    spdlog::spdlog
    fmt::fmt
    glfw
    GLAD
    ImGui
    )

add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/imgui)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE TEA_DEBUG=1)
else()
target_compile_definitions(${PROJECT_NAME} PRIVATE TEA_DEBUG=0)
endif()
