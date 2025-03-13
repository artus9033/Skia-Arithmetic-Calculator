# Flow Arithmetic Calculator

A modern C++ application project for visual Simulink- / Unreal Engine Blueprint-like flow-based arithmetic calculations with a graphical interface built using GLFW, Skia and Boost Multiprecision.

Contains documentation generation with Doxygen, unit tests with Google Test and a Justfile for simplified build operations.

Business logic is implemented as a static library, and the GUI is implemented as an executable that links against the business logic library.

## Project structure

```bash
+-- build/ # Build directory
+-- docker-dev/ # Docker development container files
+-- docs/ # Documentation
|   +-- Doxyfile.in # Doxygen configuration template
|   +-- mainpage.md # Documentation main page
+-- include/ # Public headers
|   +-- business_logic/ # Core logic headers
|   +-- gui_app/ # GUI application headers
+-- src/ # Source files
|   +-- business_logic/ # Core logic implementation
|   +-- gui_app/ # GUI application implementation
+-- tests/ # Test files
+-- third_party/ # External dependencies
+-- CMakeLists.txt # Root CMake configuration
+-- config-windows.cmake # Windows-specific configuration
+-- Justfile # Justfile for simplified build operations
+-- README.md # This file
+-- cspell.json # CSpell configuration
+-- docker-compose.yml # Docker compose configuration
+-- .clang-format # Clang format configuration
+-- .clang-tidy # Clang tidy configuration
+-- .gitignore # Git ignore file
```

## Prerequisites

- CMake 3.20 or higher
- clang & clang++
- Git
- Python 3.x (required by Skia build scripts)
- Doxygen installed
- Qt6 installed (MacOS & Linux only, **needed both for runtime and build time**)
- `clang` and `clang++` installed (**especially Windows**: `winget install -e --id LLVM.LLVM`)
- `lldb` (if debugging)

Platform-specific requirements:

- harfbuzz installed (MacOS only)
- X11, XCB & XKB development libraries installed (Linux only)
- fontconfig installed (Linux only) - required for font loading
- (**Windows only**) configured the path to Qt installation's `cmake` directory in [`config-windows.cmake`](/config-windows.cmake) that can be copied over from [`config-windows.cmake.example`](/config-windows.cmake.example)

The project automatically downloads and builds all other dependencies:

- GLFW
- Skia
- Google Test

For font rendering

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

Debug scenarios attach a debugger - `lldb`.

## Simplified scripts - Just

This project uses [Just](https://github.com/casey/just) to simplify all operations, centralizing them in a single file.

The available variables are:

- `build-type` with possible values: `"Debug"`, `"Release"` that distinguishes between debug and release builds

The available commands are:

- `just configure` - Configure the project
- `just build-all` - Build everything
- `just build-gui` - Build the GUI application
- `just build-tests` - Build the tests
- `just build-docs` - Build the HTML documentation ([`build/docs/html/index.html`](build/docs/html/index.html))
- `just build-docs-pdf` - Build the PDF documentation ([`build/docs/latex/refman.pdf`](/build/docs/latex/refman.pdf))
- `just run-gui` - Run the GUI application
- `just run-tests` - Run the tests
- `just clang-tidy` - Run clang-tidy
- `just clang-format` - Run clang-format
- `just cppcheck` - Run cppcheck
- `just cspell` - Run CSpell
- `dev-docker-up` - Run the development container
- `dev-docker-down` - Stop the development container

The general syntax is `just build-type="{Debug,Release}" command1 [? ...commands]`

**For an initial setup**, run: `just configure build-all`.

**The basic build-and-run command** you will likely be interested in is: `just build-type=Release build-gui run-gui`.

## Development container

For simplicity, the repository includes a Docker development container image file in [`docker-dev/Dockerfile`](/docker-dev/Dockerfile) and a [`docker-compose.yml`](/docker-compose.yml) compose config that can be used to bootstrap the container with the current directory (root of this repository mounted) without the `build` directory. This effectively means that for that container, the `build` directory will be separately stored inside the container disk, allowing for the host to also have its own build tree.

Justfile provides scripts `dev-docker-up` (start container) and `dev-docker-down` (stop container) for a quick usage of this container.

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

Doxygen-style comments are used for generating the documentation.

## Project components

### Business logic library

The core functionality is implemented as a static library in `src/business_logic/`. This library contains the application's main logic and is independent of the GUI implementation.

### GUI application

The GUI application in `src/gui_app/` implements the user interface using GLFW and Skia. It depends on the business logic library.

## CI / CD pipelines

The project contains a GitHub Actions [workflow](/.github/workflows/ci-cd.yml) that builds and tests the project on every push to the repository.

### CI

The continuous integration job checks the spelling with CSpell, Runs cppcheck on the files, runs clang-format ~~and clang-tidy~~. The runner used is `ubuntu-latest`.

### CD

The first continuous deployment job (`Build artifacts (CD)`) builds the project and runs the tests, only if the push occurred to the `main` branch. The job is a matrix, which runs on `ubuntu-latest`, `macos-latest`, and `windows-latest` runners.

Built application binary for each platform is uploaded as an artifact. If a push occurs with a tag, a release is created for that tag.

Documentations is also built by this job and stored as an artifact for GH Pages in the next job.

The last job (`Deploy to GitHub Pages (CD)`) deploys the documentation built in the previous job to [Github Pages](https://artus9033.github.io/Skia-Arithmetic-Calculator/).

## GIT Hooks

CMake will install the following git hooks:

- `pre-commit` (source: `pre-commit.in`) - runs `just` commands:
  - `check-clang-format` - linting
  - `check-spelling` - spellcheck
- `pre-push` (source: `pre-push.in`) - runs `just` commands:
  - `configure build-tests run-tests` (unit tests)
  - `cppcheck` (static code analysis)
