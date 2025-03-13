set windows-powershell := true

# Default to Release if not specified
build-type := "Release"

help:
    just --list

export CC := "clang"
export CXX := "clang++"

# build configuration command
configure:
    cmake -G Ninja -B build -S . -D CMAKE_BUILD_TYPE={{build-type}}

# core build commands
build-all:
    @echo "== Building all ({{build-type}}) =="
    cmake --build build --config {{build-type}}
    cmake --build build --target docs

# build the gui_app
build-gui:
    @echo "== Building gui_app ({{build-type}}) =="
    cmake --build build --target gui_app --config {{build-type}}

# build the tests
build-tests:
    @echo "== Building tests ({{build-type}}) =="
    cmake --build build --target run_tests --config {{build-type}}

# build the HTML docs with Doxygen
build-docs:
    @echo "== Building docs =="
    cmake --build build --target docs

# build the PDF docs with Doxygen
build-docs-pdf:
    @echo "== Building docs =="
    cmake --build build --target docs_pdf

# check spelling of src/** with CSpell
check-spelling:
    @echo "== Checking spelling =="
    cspell "src/**"

# run the gui_app
run-gui:
    @echo "== Running gui_app ({{build-type}}) =="
    ./build/bin/gui_app

# run the tests
run-tests:
    @echo "== Running tests ({{build-type}}) =="
    ./build/bin/run_tests

# below: source files for clang-tidy and clang-format; disabled feature on windows
sourceFiles := if os_family() == 'unix' {
    shell('find ./src -name "*.cpp" -or -name "*.h" | xargs echo')
} else {
    ""
}

# run clang-tidy
[unix]
clang-tidy:
    # generate build/compile_commands.json
    just configure
    # -p to use build/compile_commands.json
    clang-tidy -format-style=file -header-filter='^src/.*' -p build {{sourceFiles}}

# run cppcheck
[unix]
cppcheck:
    cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem \
        --suppress=unmatchedSuppression --library=zlib -I src/ -I src/business_logic -I src/gui_app \
        -I build/_deps/spd-src/include --suppress='*:build/_deps/*' \
        --config-exclude=build/_deps --check-level=exhaustive --inline-suppr --force src/

# check clang-format
[unix]
check-clang-format:
    # Check if all files are formatted correctly
    # Fail if any files need formatting
    if ! clang-format --dry-run --Werror --verbose {{sourceFiles}}; then echo "Some files are not formatted correctly. Please run clang-format."; exit 1; fi; echo "All files are properly formatted."

# run the development container
dev-docker-up:
    docker compose up -d --build

# stop the development container
dev-docker-down:
    docker compose down
