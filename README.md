# Traffic Racer C++ 🏎️

A fast-paced, vertical scrolling arcade racing game written in C++. Dodge traffic, weave through lanes, and survive as long as possible to achieve the highest score.

![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Status](https://img.shields.io/badge/status-In%20Development-yellow)

## 📖 Overview

Traffic Racer is an endless runner style game where the player controls a vehicle on a three-lane highway. The goal is to travel as far as possible while avoiding incoming traffic. The game features smooth, physics-based lane transitions rather than instant snapping, creating a satisfying "drifting" feel.

## ✨ Key Features

* **Three-Lane System:** Fixed lane centers (Left, Middle, Right) for precise horizontal movement.
* **Smooth Movement Physics:** Uses linear interpolation for a gliding effect between lanes.
* **Dynamic Difficulty:** Traffic density and speed increase naturally as the game creates a higher sense of urgency over time.
* **Scoring System:**
    * Distance-based base score.
    * **Near-Miss Bonuses:** Earn extra points for passing cars closely without crashing.
    * **Combo Multipliers:** Chain near-misses for massive points.

## 🛠️ Technical Implementation

### Smooth Lane-Switching (Gliding)
The core movement relies on a target-based smoothing algorithm, preventing instant snapping and providing a polished game feel.

**The Formula:**
$$currentX += (targetX - currentX) \times 0.15$$

### Traffic & Collision Logic
* **Spawning:** Vehicles spawn at random intervals in random lanes.
* **Collision:** Simple Bounding-Box (AABB) collision detection triggers an immediate Game Over.
* **Near-Miss:** A secondary bounding check is used to reward the player for high-risk maneuvers.

## 📂 Project Structure

The project uses CMake for building and a clean separation of source files, assets, and configuration. All source (`.cpp`) and header (`.hpp`) files are organized together within the `src/` directory.

```text
TrafficRacer/
├── assets/             # Game resources (e.g., player.png)
├── .vscode/            # Visual Studio Code configuration
├── build/              # Output directory for compiled binaries
├── src/                # Core C++ source files
│   ├── Game.hpp/cpp
│   ├── main.cpp
│   ├── Player.hpp/cpp
│   ├── Road.hpp/cpp    # Handles the continuous vertical scrolling background
│   ├── Vehicle.hpp/cpp
│   └── VehicleManager.hpp/cpp
├── CMakeLists.txt      # Build configuration for CMake
└── README.md
