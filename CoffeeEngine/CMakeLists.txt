project(Coffee-Engine VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

# Workaround for a bug in rapidjson that causes a crash in release mode
if(UNIX)
    #Currently assimp crashes in full release build

    # Remove the /O2 optimization flag from the default Release flags
    string(REPLACE "-O2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O2" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    # Set lower optimization level for Release mode (for MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")  # Optimize for size (lower optimization than /O2)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")

    # Remove the /O2 optimization flag from the default RelWithDebInfo flags
    string(REPLACE "-O2" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "-O2" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    # Set lower optimization level for RelWithDebInfo mode (for MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O1")  # Optimize for size (lower optimization than /O2)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O1")
endif()

find_package(SDL3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(cereal REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(Tracy CONFIG REQUIRED)
#find_package(imgui REQUIRED)
#find_package(ImGuizmo REQUIRED)
find_package(nfd REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(Bullet REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})

add_library(coffee-engine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS} ${LUA_INCLUDE_DIR} ${BULLET_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    fmt::fmt
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
    icon_font_cpp_headers
    Wwise
    ${LUA_LIBRARIES}
    ${BULLET_LIBRARIES}
)

option(MSDF_ATLAS_MSDFGEN_EXTERNAL "Use external msdfgen" ON)

# Set this in a profile like (Release + Profile)
option(TRACY_ENABLE "Enable Tracy Profiler" ON)
option(TRACY_ON_DEMAND "Enable Tracy on-demand mode" OFF)
option(TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)" OFF)

option(NFD_PORTAL "Use xdg-desktop-portal instead of GTK" ON)

if (WIN32)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

if (UNIX)
    option(SDL_SHARED "Use shared SDL" OFF)
    option(SDL_STATIC "Use static SDL" ON)
endif()

add_subdirectory(vendor/imgui)
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/ImGuizmo)
add_subdirectory(vendor/IconFontCppHeaders)
add_subdirectory(vendor/Wwise)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE COFFEE_DEBUG=1)
    message(STATUS "COFFEE_DEBUG ENABLED!")
else()
target_compile_definitions(${PROJECT_NAME} PRIVATE COFFEE_DEBUG=0)
message(STATUS "COFFEE_DEBUG DISABLED!")
endif()

add_definitions(-DUNICODE -D_UNICODE)