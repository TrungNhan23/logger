#!/usr/bin/env bash

# Purpose:
#   Set up local Docker dependencies for this project.
#
# Responsibilities:
#   1. Ensure the PlantUML (puml) Docker image is available.
#      - Pull the image if it does not exist locally.
#      - Start the container (or restart if already created).
#   2. Build the Docker image(s) defined in this project.
#
# Usage:
#   ./docker_prepare.sh
#
# Preconditions:
#   - Docker is installed and running.
#   - The user has permission to run Docker commands.

set -euo pipefail

PUML_IMAGE="plantuml/plantuml-server:jetty"
PUML_CONTAINER="puml-server"
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

echo "Checking for PlantUML image..."
if ! docker image inspect "$PUML_IMAGE" >/dev/null 2>&1; then
  echo "PlantUML image not found. Pulling..."
  docker pull "$PUML_IMAGE"
else
  echo "PlantUML image already available."
fi

echo "Starting PlantUML container..."
if docker ps -a --format '{{.Names}}' | grep -q "^${PUML_CONTAINER}$"; then
  docker start "$PUML_CONTAINER"
else
  docker run -d \
    --name "$PUML_CONTAINER" \
    -p 8080:8080 \
    "$PUML_IMAGE"
fi

echo "Building project Docker image(s)..."
bash $SCRIPT_DIR/build_docker.sh

echo "Prepare completed!"
