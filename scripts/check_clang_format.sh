#!/usr/bin/env bash

# === CONFIG ===
CLANG_FORMAT_BIN="clang-format"
SOURCE_EXTENSIONS=("*.c" "*.cpp" "*.h" "*.hpp")

# === FUNCTION ===
fix_file() {
    local file="$1"

    echo "Formatting: $file"
    $CLANG_FORMAT_BIN -i "$file"
}

# === MAIN ===
echo "  Auto fixing code format using clang-format..."

for ext in "${SOURCE_EXTENSIONS[@]}"; do
    while IFS= read -r file; do
        [[ -z "$file" ]] && continue
        fix_file "$file"
    done < <(find . -type f -name "$ext")
done

echo "  Done! All source files have been formatted."
