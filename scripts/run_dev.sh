#!/bin/bash
set -e

REPO_ROOT_DIR=$(git rev-parse --show-toplevel)
CONTAINER_NAME=development-env
IMAGE_NAME=development-env

# no param → start container interactive
if [ $# -eq 0 ]; then
  docker run -it \
    -v /var/run/docker.sock:/var/run/docker.sock \
    -v ${REPO_ROOT_DIR}:/workspace \
    -w /workspace \
    ${IMAGE_NAME} \
    bash
  exit 0
fi

# if param is "CI=ON" → start container in detached mode for CI
if [ "$1" = "CI=ON" ]; then
  docker run -d \
    --name ${CONTAINER_NAME} \
    -v ${GITHUB_WORKSPACE}:/workspace \
    -w /workspace \
    ${IMAGE_NAME} \
    sleep infinity
  exit 0
fi

# if param is a script → run it inside the container
docker exec ${CONTAINER_NAME} "$@"