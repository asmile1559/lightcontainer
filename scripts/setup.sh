#!/bin/bash

set -e

if ! ip link show br$1 &>/dev/null; then
    echo "br$1 not exist, add it"

    ip link add name br0 type bridge
    ip link set dev br$1 up

    echo "br$1 is running"
fi