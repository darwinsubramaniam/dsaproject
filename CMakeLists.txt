cmake_minimum_required(VERSION 3.15)

project(ELearningDashboard
    VERSION 1.0.0
    DESCRIPTION "MECS0023 E-Learning Dashboard mini project"
    LANGUAGES CXX)

# --- Language standard -------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for IDEs / clangd.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Size / dead-code elimination (Release only) -----------------------
# The default (no build type) stays unoptimized for fast compiles. All the
# size/tree-shaking work below is gated to the Release configuration; build it
# with:  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#
# Per-function/data sections let the linker discard unreferenced code; applied
# only in Release (via a generator expression) so FTXUI is built the same way.
if(NOT MSVC)
    add_compile_options(
        "$<$<CONFIG:Release>:-ffunction-sections>"
        "$<$<CONFIG:Release>:-fdata-sections>")
endif()

# Link-time optimization (cross-library dead-code elimination) for Release only
# -- this is the slow part, so it must not affect the default build.
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_message)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

# --- Vendored header-only dependency (rapidcsv) ------------------------
# Marked SYSTEM so its warnings don't show up in our strict build.
add_library(rapidcsv INTERFACE)
target_include_directories(rapidcsv SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# --- FTXUI (terminal UI), fetched and built automatically --------------
include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9)
set(FTXUI_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(FTXUI_BUILD_DOCS OFF CACHE INTERNAL "")
set(FTXUI_BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(ftxui)

# --- Executable --------------------------------------------------------
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(dashboard ${SOURCES})

target_include_directories(dashboard PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(dashboard PRIVATE
    rapidcsv
    ftxui::screen
    ftxui::dom
    ftxui::component)

# --- Warnings (portable across compilers) ------------------------------
if(MSVC)
    target_compile_options(dashboard PRIVATE /W4)
else()
    target_compile_options(dashboard PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Drop unused code from the final binary (Release only) -------------
if(APPLE)
    target_link_options(dashboard PRIVATE "$<$<CONFIG:Release>:-Wl,-dead_strip>")
elseif(NOT MSVC)
    target_link_options(dashboard PRIVATE "$<$<CONFIG:Release>:-Wl,--gc-sections>")
endif()

# Strip symbols from optimized builds for a smaller binary.
if(CMAKE_STRIP AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET dashboard POST_BUILD
        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:dashboard>
        COMMENT "Stripping symbols from dashboard")
endif()
