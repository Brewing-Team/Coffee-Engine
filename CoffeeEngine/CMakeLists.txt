project(Coffee-Engine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
#find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(cereal REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(Tracy CONFIG REQUIRED)
#find_package(imgui REQUIRED)
#find_package(ImGuizmo REQUIRED)
find_package(nfd REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(coffee-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    fmt::fmt
    #glfw
    SDL3::SDL3
    glm::glm
    GLAD
    imgui
    #imguizmo::imguizmo
    ImGuizmo
    stb_image
    assimp::assimp
    EnTT::EnTT
    Tracy::TracyClient
    nfd::nfd
    )

# Set this in a profile like (Release + Profile)
option(TRACY_ENABLE "Enable Tracy Profiler" ON)
option(TRACY_ON_DEMAND "Enable Tracy on-demand mode" OFF)
option(TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)" OFF)

option(NFD_PORTAL "Use xdg-desktop-portal instead of GTK" ON)

if (UNIX)
    option(SDL_SHARED "Use shared SDL" OFF)
    option(SDL_STATIC "Use static SDL" ON)
endif()

add_subdirectory(vendor/SDL3)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/ImGuizmo)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE COFFEE_DEBUG=1)
    message(STATUS "COFFEE_DEBUG ENABLED!")
else()
target_compile_definitions(${PROJECT_NAME} PRIVATE COFFEE_DEBUG=0)
message(STATUS "COFFEE_DEBUG DISABLED!")
endif()
