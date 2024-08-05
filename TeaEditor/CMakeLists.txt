project(Tea-Editor VERSION 0.1.0 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PANEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Panels")


file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PANEL_DIR}/*.cpp")

SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

# Set the output directory based on the project name and build type
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/$<CONFIG>")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    tea-engine)


# Copy Assets folder
add_custom_target(copy_resources ALL

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets

        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/imgui.ini
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.ini

        COMMENT "Copying resources into binary directory")

add_dependencies(${PROJECT_NAME} copy_resources)

#
