#!/usr/bin/env bash
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR="$SCRIPT_DIR/../build"
OUTPUT_FILE="$BUILD_DIR/coverage.info"
REPORT_DIR="$BUILD_DIR/coverage_report"

echo "=== Capturing coverage ==="

lcov --capture \
    --directory "$BUILD_DIR" \
    --output-file "$OUTPUT_FILE"

echo "=== Filtering unwanted paths ==="

lcov --remove "$OUTPUT_FILE" \
    '/usr/*' \
    '*/third-party/*' \
    '*/build/test/*' \
    '*/test/*' \
    '*/include/*' \
    --output-file "$OUTPUT_FILE"

echo "=== Coverage summary ==="

lcov --list "$OUTPUT_FILE"

echo "=== Generating HTML report ==="

genhtml "$OUTPUT_FILE" \
    --output-directory "$REPORT_DIR"

echo "Coverage report generated in: $REPORT_DIR/index.html"