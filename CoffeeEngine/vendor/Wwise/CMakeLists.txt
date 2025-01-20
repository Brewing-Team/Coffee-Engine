cmake_minimum_required(VERSION 3.15)
project(Wwise VERSION 0.1.0 LANGUAGES C CXX)

# Ensure WWISESDK environment variable is set
if (NOT DEFINED ENV{WWISESDK})
    message(FATAL_ERROR "WWISESDK environment variable is not set. Please set it to the Wwise SDK path.")
endif()

# Convert the WWISESDK environment variable to a CMake path
file(TO_CMAKE_PATH "$ENV{WWISESDK}" WWISE_SDK_DIR)

# Verify the path exists
if (NOT EXISTS "${WWISE_SDK_DIR}")
    message(FATAL_ERROR "The specified WWISE_SDK_DIR does not exist: ${WWISE_SDK_DIR}")
endif()

set(SAMPLES_DIR "${WWISE_SDK_DIR}/samples")

# Gather sources
file(GLOB_RECURSE SOURCES
    "${SAMPLES_DIR}/SoundEngine/Common/*.h"
    "${SAMPLES_DIR}/SoundEngine/Common/*.cpp"
    "${SAMPLES_DIR}/SoundEngine/Win32/*.h"
    "${SAMPLES_DIR}/SoundEngine/Win32/*.cpp"
)

# Add a library target for Wwise
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Include Wwise headers
target_include_directories(${PROJECT_NAME}
    PUBLIC "${WWISE_SDK_DIR}/include"
              "${SAMPLES_DIR}/SoundEngine/Common"
              "${SAMPLES_DIR}/SoundEngine/Win32"
)

# Platform-specific configurations (adjust as needed)
if (WIN32)
    set(WWISE_LIB_DIR "${WWISE_SDK_DIR}/x64_vc170/Debug/lib")
    file(GLOB LIB_FILES ${WWISE_LIB_DIR}/*.lib)
    #target_link_directories(${PROJECT_NAME} PUBLIC "${WWISE_LIB_DIR}")

    # Link necessary libraries
    target_link_libraries(${PROJECT_NAME}
            INTERFACE
            ${LIB_FILES}
            ws2_32.dll
    )
elseif(UNIX)
    set(WWISE_LIB_DIR "${WWISE_SDK_DIR}/Linux/x64/Debug/lib")
    target_link_directories(${PROJECT_NAME} INTERFACE "${WWISE_LIB_DIR}")

    target_link_libraries(${PROJECT_NAME}
        INTERFACE libAkSoundEngine.so
                  libAkMusicEngine.so
                  libAkStreamMgr.so
    )
endif()