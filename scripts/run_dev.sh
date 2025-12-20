#!/bin/bash

REPO_ROOT_DIR=$(git rev-parse --show-toplevel)

docker run -it \
  -v /var/run/docker.sock:/var/run/docker.sock \
  -v ${REPO_ROOT_DIR}:/workspace \
  -w /workspace \
  development-env bash
