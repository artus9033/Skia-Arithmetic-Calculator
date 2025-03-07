# Flow Arithmetic Calculator

A modern C++ application project for visual Simulink- / Unreal Engine Blueprint-like flow-based arithmetic calculations with a graphical interface built using GLFW, Skia and Boost Multiprecision.

Contains documentation generation with Doxygen, unit tests with Google Test and a Justfile for simplified build operations.

Business logic is implemented as a static library, and the GUI is implemented as an executable that links against the business logic library.

## Project structure

```bash
├── CMakeLists.txt # Root CMake configuration
├── docs/ # Documentation
│ ├── Doxyfile.in # Doxygen configuration template
│ └── mainpage.md # Documentation main page
├── include/ # Public headers
│ ├── business_logic/ # Core logic headers
│ └── gui_app/ # GUI application headers
├── src/ # Source files
│ ├── business_logic/ # Core logic implementation
│ └── gui_app/ # GUI application implementation
├── tests/ # Test files
└── third_party/ # External dependencies
```

## Prerequisites

- CMake 3.20 or higher
- C++20 compatible compiler
- Git
- Python 3.x (required by Skia build scripts)
- Boost installed
- Doxygen installed
- harfbuzz installed
- Qt6 installed

The project automatically downloads and builds all other dependencies:

- GLFW
- Skia
- Google Test

## Quickstart

To quickly have it all running, execute:

```bash
just configure build-all run-tests run-gui
```

## VS Code scenarios

The project carries the following preconfigured launch scenarios:

- GUI App (Debug)
- GUI App (Release)
- Tests (Debug)
- Tests (Release)

Debug scenarios attach a debugger - `lldb` on MacOS (since GDB is not supported by Apple Silicon chips) and `gdb` on other platforms.

## Simplified scripts - Just

This project uses [Just](https://github.com/casey/just) to simplify all operations, centralizing them in a single file.

The available variables are:

- `build-type` with possible values: `"Debug"`, `"Release"` that distinguishes between debug and release builds

The available commands are:

- `just configure` - Configure the project
- `just build-all` - Build everything
- `just build-gui` - Build the GUI application
- `just build-tests` - Build the tests
- `just build-docs` - Build the documentation
- `just run-gui` - Run the GUI application
- `just run-tests` - Run the tests

The general syntax is `just build-type="{Debug,Release}" command1 [? ...commands]`

**For an initial setup**, run: `just configure build-all`.

**The basic build-and-run command** you will likely be interested in is: `just build-type=Release build-gui run-gui`.

## Building

### Basic build

Create and enter build directory

```bash
mkdir build && cd build
```

Configure project

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Build everything (app, tests, docs)

```bash
cmake --build . --config Release
```

### Build options

The following CMake options are available:

- `BUILD_TESTS=ON/OFF` (default: ON) - Build unit tests
- `BUILD_DOCS=ON/OFF` (default: ON) - Generate documentation

Example with options:

```bash
cmake -DBUILD_TESTS=OFF -DBUILD_DOCS=ON ..
```

Build just the GUI application

```bash
cmake --build . --target gui_app
```

Build just tests

```bash
cmake --build . --target run_tests
```

Just generate the documentation

```bash
cmake --build . --target docs
```

## Running

After building, the executable can be found at:

- Windows: `build/src/gui_app/Release/gui_app.exe`
- Linux/MacOS: `build/src/gui_app/gui_app`

## Testing

Tests are implemented using Google Test framework. To run the tests:

```bash
cmake --build . --target run_tests
ctest -C Release --output-on-failure
```

## Project tooling, code style & static analysis

The project uses the following tools for code style:

- `clang-format` for formatting the code (configured in [`.clang-format`](.clang-format))
- `clang-tidy` for static analysis (configured in [`.clang-tidy`](.clang-tidy))
- `cspell` for spell checking (configured in [`cspell.json`](cspell.json))

## Documentation

Documentation is generated using Doxygen from source code comments. After building the `docs` target, open `build/docs/html/index.html` in your web browser.

## Project components

### Business logic library

The core functionality is implemented as a static library in `src/business_logic/`. This library contains the application's main logic and is independent of the GUI implementation.

### GUI application

The GUI application in `src/gui_app/` implements the user interface using GLFW and Skia. It depends on the business logic library.

### Documentation style

Doxygen-style comments are used for generating the documentation.
