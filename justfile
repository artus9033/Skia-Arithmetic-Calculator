set windows-powershell := true

help:
    just --list

configure:
    cd build && cmake -DCMAKE_BUILD_TYPE=Release ..

build-all:
    cmake --build . --config Release

build-gui:
    cmake --build . --target gui_app

build-tests:
    cmake --build . --target run_tests

build-docs:
    cmake --build . --target docs

run-gui:
    cmake --build . --target gui_app
    ./build/src/gui_app/gui_app

run-tests:
    cmake --build . --target run_tests
    ./build/src/tests/run_tests
