cmake_minimum_required(VERSION 3.16)
project(TrafficRacer)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 3 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(include)

# Source files
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

# Link SFML
target_link_libraries(TrafficRacer sfml-graphics sfml-window sfml-system)