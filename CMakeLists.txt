cmake_minimum_required(VERSION 3.21.0)
project(Coffee-Engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO: Change the ARCHIVE AND LIBRARY DIRECTIORES TO RESPECT THE PROJECT FOLDERS
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (MSVC)
    add_compile_options(/bigobj) # Check if we can remove this [LuaBackend.obj is too big]
endif()

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(CoffeeEngine)
add_subdirectory(CoffeeEditor)
add_subdirectory(Sandbox)
add_subdirectory(docs)