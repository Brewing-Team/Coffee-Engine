project(WWISE VERSION 0.1.0 LANGUAGES C CXX)

file(TO_CMAKE_PATH "$ENV{WWISESDK}" WWISE_SDK_DIR)

set(LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Debug/lib ${WWISE_SDK_DIR}/x64_vc170/Debug/lib)

file(GLOB LIB_FILES ${LIB_DIRS}/*.lib)

file(GLOB_RECURSE WWISE_SOURCES
        "${WWISE_SDK_DIR}/include/*.h"
)

add_library(${PROJECT_NAME} STATIC ${WWISE_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC "${WWISE_SDK_DIR}/include"
)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${LIB_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)