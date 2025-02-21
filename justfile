set windows-powershell := true

help:
    just --list

configure:
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

configure-debug:
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

build-all:
    echo "Building all in Release mode"
    cmake --build build --config Release
    just build-docs

build-all-debug:
    echo "Building all in Debug mode"
    cmake --build build --config Debug
    just build-docs

build-gui:
    echo "Building gui_app"
    cmake --build build --target gui_app

build-tests:
    echo "Building tests"
    cmake --build build --target run_tests

build-docs:
    echo "Building docs"
    cmake --build build --target docs

run-gui:
    echo "Building & running gui_app"
    just build-gui
    ./build/bin/gui_app

run-tests:
    echo "Building & running tests"
    just build-tests
    ./build/bin/run_tests
