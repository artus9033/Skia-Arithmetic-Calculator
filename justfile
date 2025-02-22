set windows-powershell := true

# Default to Release if not specified
build-type := "Release"

help:
    just --list

# build configuration command
configure:
    cmake -B build -S . -DCMAKE_BUILD_TYPE={{build-type}}

# core build commands
build-all: configure
    @echo "== Building all ({{build-type}}) =="
    cmake --build build --config {{build-type}} -- -j12
    cmake --build build --target docs -- -j12

build-gui: configure
    @echo "== Building gui_app ({{build-type}}) =="
    cmake --build build --target gui_app --config {{build-type}} -- -j12

build-tests: configure
    @echo "== Building tests ({{build-type}}) =="
    cmake --build build --target run_tests --config {{build-type}} -- -j12

build-docs: configure
    @echo "== Building docs =="
    cmake --build build --target docs -- -j12

# run commands
run-gui: build-gui
    @echo "== Running gui_app ({{build-type}}) =="
    ./build/bin/gui_app

run-tests: build-tests
    @echo "== Running tests ({{build-type}}) =="
    ./build/bin/run_tests
