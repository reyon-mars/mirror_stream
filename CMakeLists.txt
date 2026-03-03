cmake_minimum_required(VERSION 3.20)

project(mirror_stream VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TUI "Build Terminal UI Version" ON)
option(BUILD_GUI "Build GUI version" OFF)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# --------------------------
# Core Library
# --------------------------

add_library(core STATIC
    src/net/connection.cpp
    src/net/socket.cpp
    src/utils/logger.cpp
    src/utils/utils.cpp
)

target_include_directories(core PUBLIC include)

if(UNIX)
    target_link_libraries(core PUBLIC pthread)
endif()

target_compile_features(core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(core PRIVATE /W4 /permissive-)
else()
    target_compile_options(core PRIVATE -Wall -Wextra -pedantic -Wshadow -Wconversion)
endif()

# --------------------------
# TUI Executable
# --------------------------

if(BUILD_TUI)

    add_executable(mirror_stream_tui)
    target_sources(
        src/ui/input_handler.cpp
        src/ui/menu_renderer.cpp
        src/ui/terminal_ui.cpp
        src/main_tui.cpp
    )

    target_link_libraries(mirror_stream_tui PRIVATE core)

    install(TARGETS mirror_stream_tui DESTINATION bin)

endif()


# --------------------------
# GUI Executable
# --------------------------

if(BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    add_executable(mirror_stream_gui)

    target_sources(mirror_stream_gui
        src/gui/config_widget.cpp
        src/gui/main_window.cpp
        src/main_gui.cpp
    )

    target_link_libraries(
        PRIVATE
        core
        Qt6::Widgets
    )

    install(TARGETS mirror_stream_gui DESTINATION bin)
endif()
