#!/usr/bin/env bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
OUTPUT_FILE="$BUILD_DIR/coverage.info"
REPORT_DIR="$BUILD_DIR/coverage_report"

chmod -R u+w "$BUILD_DIR"
echo "=== Capturing coverage ==="

lcov --capture \
    --directory "$BUILD_DIR" \
    --output-file "$OUTPUT_FILE"

echo "=== Filtering unwanted paths ==="

lcov --remove "$OUTPUT_FILE" \
    '/usr/*' \
    '*/third-party/*' \
    '*/build/*' \
    '*/test/*' \
    '*/example/*' \
    --output-file "$OUTPUT_FILE"

echo "=== Coverage summary ==="

lcov --list "$OUTPUT_FILE"

echo "=== Generating HTML report ==="

genhtml "$OUTPUT_FILE" \
    --output-directory "$REPORT_DIR"

echo "Coverage report generated in: $REPORT_DIR/index.html"