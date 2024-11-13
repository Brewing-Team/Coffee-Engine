project(icon_font_cpp_headers VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} STATIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/IconsLucide.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/IconsLucide.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")