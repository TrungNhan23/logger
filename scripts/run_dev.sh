#!/bin/bash
set -e

REPO_ROOT_DIR=$(git rev-parse --show-toplevel)
HOME_DIR="$HOME"
CONTAINER_NAME=development-env
IMAGE_NAME=development-env
HISTORY_FILE="$HOME/.docker_bash_history"

function get_docker_history_command()
{
  if [ ! -f "$HISTORY_FILE" ]; then
    touch "$HISTORY_FILE"
  fi
}

get_docker_history_command

# no param → start container interactive
if [ $# -eq 0 ]; then
  docker run -it \
    -v /var/run/docker.sock:/var/run/docker.sock \
    -v "$HISTORY_FILE":/root/.bash_history \
    -v ${REPO_ROOT_DIR}:${REPO_ROOT_DIR} \
    -w ${REPO_ROOT_DIR} \
    -e PROJECT_ROOT=${REPO_ROOT_DIR} \
    ${IMAGE_NAME} \
    bash
  exit 0
fi

# if param is "CI=ON" → start container in detached mode for CI
if [ "$1" = "CI=ON" ]; then
  docker run -d \
    --name ${CONTAINER_NAME} \
    -v ${REPO_ROOT_DIR}:${REPO_ROOT_DIR} \
    -w ${REPO_ROOT_DIR} \
    -e PROJECT_ROOT=${REPO_ROOT_DIR} \
    ${IMAGE_NAME} \
    sleep infinity
  exit 0
fi

# if param is a script → run it inside the container
docker exec ${CONTAINER_NAME} "$@"