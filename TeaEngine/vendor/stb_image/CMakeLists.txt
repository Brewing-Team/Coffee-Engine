project(stb_image VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp")

target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
