#!/usr/bin/env bash

# === CONFIG ===
CLANG_FORMAT_BIN="clang-format"
SOURCE_EXTENSIONS=("*.c" "*.cpp" "*.h" "*.hpp")

# === READ SUBMODULE PATHS ===
SUBMODULE_PATHS=()

if [[ -f .gitmodules ]]; then
    echo "Reading submodule paths..."
    while IFS= read -r line; do
        # Only extract actual path
        path=$(echo "$line" | awk '{print $3}')
        SUBMODULE_PATHS+=("./$path")
    done < <(grep "path =" .gitmodules)
fi

echo "Ignoring submodules: ${SUBMODULE_PATHS[*]}"

# === FORMAT FUNCTION ===
fix_file() {
    local file="$1"
    echo "Formatting: $file"
    $CLANG_FORMAT_BIN -i "$file"
}

# === MAIN ===
echo "Auto fixing code format using clang-format..."

for ext in "${SOURCE_EXTENSIONS[@]}"; do

    # Build prune part
    prune=""
    for sm in "${SUBMODULE_PATHS[@]}"; do
        prune="$prune -path \"$sm\" -prune -o"
    done

    # Run find with proper pruning
    eval "find . $prune -type f -name \"$ext\" -print" | while IFS= read -r file; do
        [[ -z "$file" ]] && continue
        fix_file "$file"
    done

done

echo "Done! All non-submodule source files have been formatted."
