set windows-powershell := true

help:
    just --list

configure:
    cmake -B build -S .

build-all:
    @echo "== Building all in Release mode ==\n"
    cmake --build build --config Release
    just build-docs

build-all-debug:
    @echo "== Building all in Debug mode ==\n"
    cmake --build build --config Debug
    just build-docs

build-gui:
    @echo "== Building gui_app ==\n"
    cmake --build build --target gui_app

build-tests:
    @echo "== Building tests ==\n"
    cmake --build build --target run_tests

build-docs:
    @echo "== Building docs ==\n"
    cmake --build build --target docs

run-gui:
    @echo "== Building & running gui_app ==\n"
    just build-gui
    ./build/bin/gui_app

run-tests:
    @echo "== Building & running tests ==\n"
    just build-tests
    ./build/bin/run_tests
