project(TeaEngine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(tea-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    fmt::fmt
    glfw
    glm::glm
    GLAD
    ImGui
    stb_image
    assimp
    EnTT::EnTT
    Tracy::TracyClient
    )

# Set this in a profile like (Release + Profile)
option(TRACY_ENABLE "Enable Tracy Profiler" ON)
option(TRACY_ON_DEMAND "Enable Tracy on-demand mode" OFF)
option(TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)" OFF)

add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/tracy)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE TEA_DEBUG=1)
else()
target_compile_definitions(${PROJECT_NAME} PRIVATE TEA_DEBUG=0)
endif()
