# Ray Tracing Engine

A relativistic ray tracing engine simulating a Schwarzschild Black Hole.

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

## Build Instructions
```bash
mkdir out
cd out
cmake ..
cmake --build . --config Debug
```
