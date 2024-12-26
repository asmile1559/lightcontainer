#ifndef LIGHTCONTAINER_NETWORK_H
#define LIGHTCONTAINER_NETWORK_H

#define _GNU_SOURCE
#include <sys/types.h>


#define CREATE_VETH_PAIR_FMT "ip link add name veth%d type veth peer name veth%d"
#define CREATE_VETH_PAIR_MV2NS_FMT "ip link add name veth%d type veth peer name veth%d netns %d"
#define SETUP_VETH_FMT "ip addr add %s dev veth%d"
#define VETH_UP_FMT "ip link set dev veth%d up"

#define CREATE_BR_FMT "ip link add name br%d type bridge"
#define BR_UP_FMT "ip link set br%d up"
#define LINK_ETH_BR_FMT "ip link set dev eth%d master br%d"
#define LINK_VETH_BR_FMT "ip link set dev veth%d master br%d"

#define DEFAULT_HOST_VETHID 0
#define DEFAULT_CONTAINER_VETHID 1

#define DEFAULT_HOST_IP "10.0.0.1/24"
#define DEFAULT_CONTAINER_IP "10.0.0.2/24"


int create_veth_pair(int veth0, int veth1);
int create_veth_pair_mv2ns(int vethID0, int vethID1, pid_t target);
int setup_veth(char *ip, int vethID);
int veth_up(int vethID);
int create_bridge(int bridgeID);
int bridge_up(int bridgeID);
int link_eth_br(int ethID, int bridgeID);
int link_veth_br(int vethID, int bridgeID);
#endif