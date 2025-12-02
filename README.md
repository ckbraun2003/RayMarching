# Ray Marching

A relativistic ray marching simulating a Schwarzschild Black Hole.

## Features
- **General Relativity**: Simulates light bending (geodesics) around a black hole.
- **Volumetric Accretion Disk**: Glowing matter swirling around the event horizon.
- **Procedural Nebula**: Colorful background clouds to visualize gravitational lensing.
- **World System**: Object-oriented scene management.

## Controls
- `WASD`: Move
- `Mouse`: Look
- `G`: Toggle CPU/GPU mode (CPU mode is basic ray casting)
- `Esc`: Close

## Installation

### Option 1: Run Pre-built (Easiest)
If you have a pre-built version (the `out/Debug` folder), simply run:
`RayTracingEngine.exe`
*Ensure `glfw3.dll` and the `shaders/` folder are in the same directory as the executable.*

### Option 2: Build from Source
To build this project on a new machine, you need:
1.  **C++ Compiler** (Visual Studio 2022 recommended on Windows)
2.  **CMake** (v3.10+)
3.  **vcpkg** (Package Manager)

#### 1. Install Dependencies (vcpkg)
```bash
# Clone vcpkg if you haven't
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat

# Install libraries
.\vcpkg\vcpkg install glfw3 glad glm
```

#### 2. Build
```bash
mkdir out
cd out
# Replace [path/to/vcpkg] with your actual vcpkg location
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
```
