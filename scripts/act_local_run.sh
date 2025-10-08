#!/bin/bash

act -P ubuntu-22.04=act-images-dev \
    --pull=false \
    --container-options "--network=host"
