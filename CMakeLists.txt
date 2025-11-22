cmake_minimum_required(VERSION 3.16)
project(TrafficRacer)
set(CMAKE_CXX_STANDARD 17)

# --- 1. MANUAL MAC CONFIGURATION ---
# Tell CMake exactly where Homebrew keeps the header files
include_directories("/opt/homebrew/include")

# Tell CMake exactly where the library files are
link_directories("/opt/homebrew/lib")

# Include your own header folder
include_directories(include)

# --- 2. SOURCE FILES ---
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Vehicle.cpp
    src/Road.cpp
    src/VehicleManager.cpp
)

# --- 3. BUILD APP ---
add_executable(TrafficRacer ${SOURCES})

# --- 4. LINK LIBRARIES ---
# CRITICAL CHANGE: Added "sfml-audio" to the end of this list
target_link_libraries(TrafficRacer sfml-graphics sfml-window sfml-system sfml-audio)