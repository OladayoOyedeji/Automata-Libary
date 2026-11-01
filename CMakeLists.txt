cmake_minimum_required(VERSION 3.31)
project(Automata VERSION 1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    src/main.cpp
    include/NFA.h
    include/DFA.h
    include/Regex.h
    include/STL_util.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_custom_target(r
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    USES_TERMINAL
)

add_custom_target(c
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    USES_TERMINAL
)
