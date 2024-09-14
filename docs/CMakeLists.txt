find_package(Doxygen REQUIRED)

# Find all the public headers
get_target_property(TEA_ENGINE_PUBLIC_HEADER_DIR TeaEngine INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE TEA_ENGINE_PUBLIC_HEADERS ${TEA_ENGINE_PUBLIC_HEADER_DIR}/*.h)

# This will be the main output of our command
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/TeaEngine/src/TeaEngine)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) # Doxygen won't create this for us

add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
    DEPENDS ${TEA_ENGINE_PUBLIC_HEADERS}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
    COMMENT "Generating docs"
)

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})