set windows-powershell := true

# Default to Release if not specified
build-type := "Release"

help:
    just --list

# build configuration command
configure:
    cmake -B build -S . -DCMAKE_BUILD_TYPE={{build-type}}

# core build commands
build-all:
    @echo "== Building all ({{build-type}}) =="
    cmake --build build --config {{build-type}} -- -j12
    cmake --build build --target docs -- -j12

build-gui:
    @echo "== Building gui_app ({{build-type}}) =="
    cmake --build build --target gui_app --config {{build-type}} -- -j12

build-tests:
    @echo "== Building tests ({{build-type}}) =="
    cmake --build build --target run_tests --config {{build-type}} -- -j12

build-docs:
    @echo "== Building docs =="
    cmake --build build --target docs -- -j12

# run commands
run-gui:
    @echo "== Running gui_app ({{build-type}}) =="
    ./build/bin/gui_app

run-tests:
    @echo "== Running tests ({{build-type}}) =="
    ./build/bin/run_tests

clang-tidy:
    # generate build/compile_commands.json
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    # -p to use build/compile_commands.json
    clang-tidy -format-style=file -header-filter=. -p build src/**/*.cpp

check-clang-format:
    # Check if all files are formatted correctly
    # Fail if any files need formatting
    if !clang-format --dry-run --Werror --verbose $(find ./src/ -name "*.cpp" -or -name "*.h"); then \
        echo "Some files are not formatted correctly. Please run clang-format." \
        exit 1 \
    fi \
    echo "All files are properly formatted."
