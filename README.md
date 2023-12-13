# Tree Generator

An application for creating trees and other vegetation using L-systems.

## Build Instructions
Build configuration is provided via CMake. On Windows, the project can be configured using one of the two build presets provided.

Dependencies are downloaded automatically via CMake's FetchContent. An external installation of Python is required for configuring glad (the OpenGL loader).

## Current Features

* Generates a tree from a user-specified axiom and rule set.
* Configurable actions that determine how the generated symbols are converted to a 3D object.

## Planned Features

* Web build via Emscripten.
* User-specified mapping of symbols to actions.
* Built-in L-system presets.
* Saving and loading L-system and mesh settings.
* Better meshing algorithm.
* Export to .obj file.
* Parameterized symbols and non-deterministic rules.
* Vulkan support.