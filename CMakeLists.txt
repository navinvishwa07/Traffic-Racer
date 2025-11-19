# CMake minimum version requirement
cmake_minimum_required(VERSION 3.16)
project(TrafficRacer CXX) # Specify CXX language for C++ project

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Find SFML
# Use the correct component names as defined by the FindSFML module
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED) 
# Note: Changed '3' to '2.5'

# 2. Include directories
# We need to include the 'src' folder where all headers are located.
include_directories(src) 
# Note: Changed 'include' to 'src'

# Source files (Looks correct based on your file structure)
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Vehicle.cpp
    src/Road.cpp
    src/VehicleManager.cpp
)

# Add executable
add_executable(TrafficRacer ${SOURCES})

# 3. Link SFML
# We must link using the CMake targets (e.g., SFML::graphics) created by find_package.
target_link_libraries(TrafficRacer PRIVATE SFML::graphics SFML::window SFML::system)
# Note: Changed 'sfml-graphics' to 'SFML::graphics' and specified PRIVATE linking.