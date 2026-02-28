cmake_minimum_required(VERSION 3.20)

project(mirror_stream VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(SOURCES
    src/main.cpp
    src/socket.cpp
    src/terminal_ui.cpp
)

add_executable(server ${SOURCES})

if(MSVC)
    target_compile_options(server PRIVATE /W4 /permissive-)
else()
    target_compile_options(server PRIVATE -Wall -Wextra -pedantic -Wshadow -Wconversion)
endif()

if(UNIX)
    target_link_libraries(server PRIVATE pthread)
endif()

install(TARGETS server DESTINATION bin)
