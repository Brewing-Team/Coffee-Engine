project(Wwise VERSION 0.1.0 LANGUAGES C CXX)

file(TO_CMAKE_PATH "$ENV{WWISESDK}" WWISE_SDK_DIR)

set(LIB_DIRS ${WWISE_SDK_DIR}/x64_vc170/Debug/lib)

file(GLOB LIB_FILES ${LIB_DIRS}/*.lib)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
        INTERFACE "${WWISE_SDK_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
        INTERFACE
        ${LIB_FILES}
        ws2_32
)