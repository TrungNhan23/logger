#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="build"
THREADS=2

echo_red ()
{
    echo -e "\033[31m$1\033[0m"
}

echo_green ()
{
    echo -e "\033[32m$1\033[0m"
}

ensure_in_docker()
{
    if [ -f /.dockerenv ] || grep -qE '(docker|containerd|kubepods)' /proc/1/cgroup 2>/dev/null; then
        echo_green "Running inside a Docker container."
    else
        echo_red "Not running inside a Docker container."
        return 127
    fi
}

configure() {
    echo_green "== Configure CMake =="
    cmake -S . -B ${BUILD_DIR} \
          -DENABLE_LINT=ON \
          -DENABLE_COVERAGE=ON \
          -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
}

lint() {
    echo_green "== Run Linter =="
    cmake --build ${BUILD_DIR} --target lint
}

build() {
    echo_green "== Build Project =="
    cmake --build ${BUILD_DIR} -- -j${THREADS}
}

run()
{
    if [ -x "build/example/main_logger" ]; then
        echo_green "Running build/example/main_logger"
        ./build/example/main_logger
    fi

    if [ -x "build/test/test_logger" ]; then
        echo_green "Running build/test/test_logger"
        ./build/test/test_logger
    fi
}

stress()
{
    echo_green "== Run Performance & Stress Tests =="
    if [ ! -x "build/bench/benchmark_logger" ]; then
        cmake --build ${BUILD_DIR} --target benchmark_logger -- -j${THREADS}
    fi
    ./build/bench/benchmark_logger
}

parse_options()
{
    case "${1:-build_and_run}" in
        configure)
            configure
            ;;
        lint)
            lint
            ;;
        build)
            build
            ;;
        run)
            run
            ;;
        stress|bench|benchmark)
            configure
            build
            stress
            ;;
        build_and_run)
            configure
            build
            run
            ;;    
        all)
            configure
            lint
            build
            run
            stress
            ;;
        *)
            echo "Usage: $0 { configure | lint | build | run | stress | build_and_run | all }"
            exit 1
            ;;
    esac
}

main()
{
    ensure_in_docker
    parse_options "$@"
}

###### Main execution starts here (do not change it) ######
main "$@"
