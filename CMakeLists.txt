cmake_minimum_required(VERSION 3.15)

project(ELearningDashboard
    VERSION 1.0.0
    DESCRIPTION "MECS0023 E-Learning Dashboard mini project"
    LANGUAGES CXX)

# --- Language standard -------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for IDEs / clangd.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Vendored header-only dependency (rapidcsv) ------------------------
# Marked SYSTEM so its warnings don't show up in our strict build.
add_library(rapidcsv INTERFACE)
target_include_directories(rapidcsv SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# --- Executable --------------------------------------------------------
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(dashboard ${SOURCES})

target_include_directories(dashboard PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(dashboard PRIVATE rapidcsv)

# --- Warnings (portable across compilers) ------------------------------
if(MSVC)
    target_compile_options(dashboard PRIVATE /W4)
else()
    target_compile_options(dashboard PRIVATE -Wall -Wextra -Wpedantic)
endif()
