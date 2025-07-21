cmake_minimum_required(VERSION 3.21.0)
project(Coffee-Engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for available linkers and use the best one available
# Priority: mold > lld > ld (default)
if(UNIX AND NOT APPLE)
    # Check for mold linker
    find_program(MOLD_LINKER mold)
    if(MOLD_LINKER)
        message(STATUS "Using mold linker: ${MOLD_LINKER}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=mold")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=mold")
    else()
        # Check for lld linker
        find_program(LLD_LINKER lld)
        if(LLD_LINKER)
            message(STATUS "Using lld linker: ${LLD_LINKER}")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=lld")
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