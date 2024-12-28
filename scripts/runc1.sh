#!/bin/bash
set -e

if [ $USER != "root" ]; then
    echo "must be the root user or in root mode "
    exit 1
fi

# rootfs
NEW_ROOTFS_PATH=""
OLD_ROOTFS_PATH=""

# host
HOST_VETH_ID="1"
HOST_VETH_IP="10.0.1.1/24"
BRIDGE_ID="0"

# container
CONTAINER_VETH_ID="2"
CONTAINER_VETH_IP="10.0.1.2/24"

# cgroups
# percent from [0 ~ core_number]
CPU_LIMIT_PERCENT=0.3
MEM_LIMIT_IN_MB=512

# do not modify the following
MB=1048576
CPU_CFS_PERIOD_US=100000
CPU_LIMIT=$(echo "scale=0; $CPU_LIMIT_PERCENT * $CPU_CFS_PERIOD_US" | bc)
MEM_LIMIT=$(echo "sacle=0; $MB * $MEM_LIMIT_IN_MB" | bc)

if [ -n $BRIDGE_ID ]; then 
    SCRIPT_DIR=$(dirname "$0")
    ./$SCRIPT_DIR/setup.sh $BRIDGE_ID
fi

./bin/lightcontainer \
    -n $NEW_ROOTFS_PATH \
    -o $OLD_ROOTFS_PATH \
    -d $CONTAINER_VETH_ID \
    -p $CONTAINER_VETH_IP \
    -D $HOST_VETH_ID \
    -P $HOST_VETH_IP \
    -B $BRIDGE_ID \
    -c $CPU_LIMIT \
    -m $MEM_LIMIT