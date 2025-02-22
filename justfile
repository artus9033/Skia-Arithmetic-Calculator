set windows-powershell := true

# Default to Release if not specified
build-type := "Release"

help:
    just --list

# Core build commands
configure:
    cmake -B build -S . -DCMAKE_BUILD_TYPE={{build-type}}

build-all: configure
    @echo "== Building all ({{build-type}}) =="
    cmake --build build --config {{build-type}}
    cmake --build build --target docs

build-gui: configure
    @echo "== Building gui_app ({{build-type}}) =="
    cmake --build build --target gui_app --config {{build-type}}

build-tests: configure
    @echo "== Building tests ({{build-type}}) =="
    cmake --build build --target run_tests --config {{build-type}}

build-docs: configure
    @echo "== Building docs =="
    cmake --build build --target docs

# Run commands
run-gui: build-gui
    @echo "== Running gui_app ({{build-type}}) =="
    ./build/bin/gui_app

run-tests: build-tests
    @echo "== Running tests ({{build-type}}) =="
    ./build/bin/run_tests
