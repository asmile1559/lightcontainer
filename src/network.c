#include "network/network.h"

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_veth_pair(veth_t *v0, veth_t *v1){
    if (!v0 || !v1 || !v0->id || !v1->id) {
        fprintf(stderr, "[ERROR] veth is NULL");
        return -1;
    }
    char cmd[100];
    sprintf(cmd, CREATE_VETH_PAIR_FMT, v0->id, v1->id);
    if (system(cmd) < 0){
        perror("create_veth_pair error");
        return -1;
    }
    v0->state = STATE_DOWN;
    v1->state = STATE_DOWN;
    return 0;
}

int create_veth_pair_mv2ns(veth_t *v0, veth_t *v1, pid_t pid){
    if (!v0 || !v1 || !v0->id || !v1->id) {
        fprintf(stderr, "[ERROR] veth is NULL");
        return -1;
    }
    char cmd[100];
    sprintf(cmd, CREATE_VETH_PAIR_MV2NS_FMT, v0->id, v1->id, pid);
    if (system(cmd) < 0){
        perror("create_veth_pair_mv2ns error");
        return -1;
    }
    v0->state = STATE_DOWN;
    v1->state = STATE_DOWN;
    return 0;
}

int setup_veth(veth_t *v){
    if (!v || !v->ip || !v->id) {
        fprintf(stderr, "[ERROR] veth is NULL");
        return -1;
    }
    char cmd[100];
    sprintf(cmd, SETUP_VETH_FMT, v->ip, v->id);
    if (system(cmd) < 0){
        perror("set_up_veth error");
        return -1;
    }
    v->state = STATE_READY;
    return 0;
}

int veth_up(veth_t *v) {
    if (!v || !v->ip || !v->id) {
        fprintf(stderr, "[ERROR] veth is NULL");
        return -1;
    }

    if (v->state == STATE_UP) {
        return 0;
    } else if (v->state != STATE_READY) {
        fprintf(stderr, "[ERROR] Need to create or setup veth before up it.");
        return -1;
    }

    char cmd[100];
    sprintf(cmd, VETH_UP_FMT, v->id);
    if (system(cmd) < 0){
        perror("veth_up error");
        return -1;
    }
    v->state = STATE_UP;
    return 0;
}

// int create_bridge(char *bridgeID){
//     char cmd[100];
//     sprintf(cmd, CREATE_BR_FMT, bridgeID);
//     if (system(cmd) < 0){
//         perror("create_bridge error");
//         return -1;
//     }
//     return 0;  
// }

// int bridge_up(char *bridgeID) {
//     char cmd[100];
//     sprintf(cmd, BR_UP_FMT, bridgeID);
//     if (system(cmd) < 0){
//         perror("bridge_up error");
//         return -1;
//     }
//     return 0;
// }

int link_veth_br(veth_t *v, char *bridgeID){
    if (!v || !bridgeID || !v->id){
        fprintf(stderr, "[ERROR] veth or bridgeID is NULL");
        return -1;
    }
    if (v->state != STATE_UP) {
        fprintf(stderr, "[WARNING] veth not up yet!");
    }
    char cmd[100];
    sprintf(cmd, LINK_VETH_BR_FMT, v->id, bridgeID);
    if (system(cmd) < 0){
        perror("link_veth_br error");
        return -1;
    }
    return 0;    
}

int link2device(char *dev1, char *dev2){
    char cmd[100];
    sprintf(cmd, LINK_2DEBICE_FMT, dev1, dev2);
    if (system(cmd) < 0){
        perror("link_veth_br error");
        return -1;
    }
    return 0;    
}