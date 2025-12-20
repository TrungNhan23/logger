#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DOCKER_DIR="${SCRIPT_DIR}/docker"

function get_image_name() {
    local filename="$1"
    # Remove directory path
    filename=$(basename "$filename")
    # Strip the trailing "-Dockerfile"
    echo "${filename%-Dockerfile}"
}

for file in "$DOCKER_DIR"/*-Dockerfile; do
    [ -e "$file" ] || continue  # Skip if no matching files

    image_name=$(get_image_name "$file")

    echo "Building image: $image_name from $file"

    docker build \
        -t "$image_name" \
        --network host \
        -f "$file" \
        .
done
