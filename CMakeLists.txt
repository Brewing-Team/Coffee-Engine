cmake_minimum_required(VERSION 3.21.0)
project(Coffee-Engine VERSION 0.4.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#option(iwyu "Run include-what-you-use" OFF)
#if(iwyu)
#  find_program(IWYU_EXE NAMES include-what-you-use REQUIRED)
#  set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
#  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
#endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache in ${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Check for available linkers and use the best one available (Linux/Unix only)
# Priority: mold > lld > ld (default)
if(UNIX AND NOT APPLE)
    # Check for mold linker
    find_program(MOLD_LINKER mold)
    if(MOLD_LINKER)
        message(STATUS "Using mold linker: ${MOLD_LINKER}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=mold")
    else()
        # Check for lld linker
        find_program(LLD_LINKER lld)
        if(LLD_LINKER)
            message(STATUS "Using lld linker: ${LLD_LINKER}")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
        else()
            message(STATUS "Using default ld linker")
            # No need to set flags for default linker
        endif()
    endif()
endif()

# TODO: Change the ARCHIVE AND LIBRARY DIRECTIORES TO RESPECT THE PROJECT FOLDERS
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (MSVC)
    add_compile_options(/bigobj) # Check if we can remove this [LuaBackend.obj is too big]
endif()

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(CoffeeEngine)
add_subdirectory(Editor)
add_subdirectory(Runtime)
add_subdirectory(Sandbox)
add_subdirectory(docs)