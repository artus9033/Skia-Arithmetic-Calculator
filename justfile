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

# build the gui_app
build-gui:
    @echo "== Building gui_app ({{build-type}}) =="
    cmake --build build --target gui_app --config {{build-type}} -- -j12

# build the tests
build-tests:
    @echo "== Building tests ({{build-type}}) =="
    cmake --build build --target run_tests --config {{build-type}} -- -j12

# build the docs with Doxygen
build-docs:
    @echo "== Building docs =="
    cmake --build build --target docs -- -j12

# run the gui_app
run-gui:
    @echo "== Running gui_app ({{build-type}}) =="
    ./build/bin/gui_app

# run the tests
run-tests:
    @echo "== Running tests ({{build-type}}) =="
    ./build/bin/run_tests

sourceFiles := shell('find ./src -name "*.cpp" -or -name "*.h" | xargs echo')

# run clang-tidy
clang-tidy:
    # generate build/compile_commands.json
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    # -p to use build/compile_commands.json
    clang-tidy -format-style=file -header-filter=. -p build {{sourceFiles}}

# check clang-format
check-clang-format:
    # Check if all files are formatted correctly
    # Fail if any files need formatting
    if ! clang-format --dry-run --Werror --verbose {{sourceFiles}}; then echo "Some files are not formatted correctly. Please run clang-format." exit 1; fi; echo "All files are properly formatted."
