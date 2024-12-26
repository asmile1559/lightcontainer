#include "network/network.h"

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_veth_pair(int vethID0, int vethID1){
    char cmd[100];
    sprintf(cmd, CREATE_VETH_PAIR_FMT, vethID0, vethID1);
    if (system(cmd) < 0){
        perror("create_veth_pair error");
        return -1;
    }
    return 0;
}

int create_veth_pair_mv2ns(int vethID0, int vethID1, pid_t target){
    char cmd[100];
    sprintf(cmd, CREATE_VETH_PAIR_MV2NS_FMT, vethID0, vethID1, target);
    if (system(cmd) < 0){
        perror("create_veth_pair_mv2ns error");
        return -1;
    }
    return 0;
}

int setup_veth(char *ip, int vethID){
    char cmd[100];
    sprintf(cmd, SETUP_VETH_FMT, ip, vethID);
    if (system(cmd) < 0){
        perror("set_up_veth error");
        return -1;
    }
    return 0;
}

int veth_up(int vethID) {
    char cmd[100];
    sprintf(cmd, VETH_UP_FMT, vethID);
    if (system(cmd) < 0){
        perror("veth_up error");
        return -1;
    }
    return 0;
}

int create_bridge(int bridgeID){
    char cmd[100];
    sprintf(cmd, CREATE_BR_FMT, bridgeID);
    if (system(cmd) < 0){
        perror("create_bridge error");
        return -1;
    }
    return 0;  
}

int bridge_up(int bridgeID) {
    char cmd[100];
    sprintf(cmd, BR_UP_FMT, bridgeID);
    if (system(cmd) < 0){
        perror("bridge_up error");
        return -1;
    }
    return 0;
}

int link_eth_br(int ethID, int bridgeID){
    char cmd[100];
    sprintf(cmd, LINK_ETH_BR_FMT, ethID, bridgeID);
    if (system(cmd) < 0){
        perror("link_eth_br error");
        return -1;
    }
    return 0;
}

int link_veth_br(int vethID, int bridgeID){
    char cmd[100];
    sprintf(cmd, LINK_VETH_BR_FMT, vethID, bridgeID);
    if (system(cmd) < 0){
        perror("link_veth_br error");
        return -1;
    }
    return 0;    
}