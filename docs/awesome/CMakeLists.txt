cmake_minimum_required(VERSION 3.31)

project(Automata
    VERSION 1.0
    DESCRIPTION "Automata Library"
    LANGUAGES C CXX
)

include(FetchContent)

FetchContent_Declare(
    doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(doxygen-awesome-css)

FetchContent_GetProperties(
    doxygen-awesome-css
    SOURCE_DIR AWESOME_CSS_DIR
)

set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file(
    "${DOXYFILE_IN}"
    "${DOXYFILE_OUT}"
    @ONLY
)
