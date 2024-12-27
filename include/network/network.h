#ifndef LIGHTCONTAINER_NETWORK_H
#define LIGHTCONTAINER_NETWORK_H

#define _GNU_SOURCE
#include <sys/types.h>


#define CREATE_VETH_PAIR_FMT "ip link add name veth%s type veth peer name veth%s"
#define CREATE_VETH_PAIR_MV2NS_FMT "ip link add name veth%s type veth peer name veth%s netns %d"
#define SETUP_VETH_FMT "ip addr add %s dev veth%s"
#define VETH_UP_FMT "ip link set dev veth%s up"

// #define CREATE_BR_FMT "ip link add name br%s type bridge"
// #define BR_UP_FMT "ip link set br%s up"
// #define LINK_ETH_BR_FMT "ip link set dev eth%s master br%s"
#define LINK_VETH_BR_FMT "ip link set dev veth%s master br%s"

#define LINK_2DEBICE_FMT "ip link set dev %s master %s"

#define DEFAULT_HOST_VETHID "0"
#define DEFAULT_CONTAINER_VETHID "1"

#define DEFAULT_HOST_IP "10.0.0.1/24"
#define DEFAULT_CONTAINER_IP "10.0.0.2/24"

#define STATE_INIT 0
#define STATE_DOWN 1
#define STATE_READY 2
#define STATE_UP 3
typedef struct 
{
    char *id;
    char *ip;
    int state;
}veth_t;


int create_veth_pair(veth_t *v1, veth_t *v2);
int create_veth_pair_mv2ns(veth_t *v1, veth_t *v2, pid_t pid);
int setup_veth(veth_t *v);
int veth_up(veth_t *v);
int link_veth_br(veth_t *v, char *bridgeID);
int link2device(char *dev1, char *dev2);

// int create_bridge(char *id);
// int bridge_up(char *bridgeID);


#endif