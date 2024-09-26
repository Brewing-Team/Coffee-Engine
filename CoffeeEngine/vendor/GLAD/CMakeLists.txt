project(GLAD VERSION 0.1.0 LANGUAGES C CXX)

set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
#add_library(GLAD ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
