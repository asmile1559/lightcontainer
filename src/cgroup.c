#define _GNU_SOURCE

#include "cgroup/cgroup.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

cgroupctl cgctl;

int init_cgroupctl(resource_type t){
    cgctl.cgroup_root_path = CGROUP_ROOT_PATH;
    cgctl.t = t;
    for (int i = 0; i < CGROUP_CTR_NUM; i++){
        cgctl.ss[i] = 0;
        int msk = 1 << i;
        if (t & msk) {
            // printf("msk=%d\n", msk);
            cgctl.ss[i] = init_subsystem(msk);
            if (cgctl.ss[i] == NULL) {
                return -1;
            }
        }
    }
    return 0;
}

int cg_set(resource_type t, char *resource){
    if (!(t & cgctl.t)){
        fprintf(stderr, "set a invalid cgroup\n");
        return -1;
    } 
    subsystem *ss = cgctl.ss[bit2num(t)];
    if (ss == NULL) {
        printf("no!\n");
        return -1;
    }
    return subsystem_set(ss, resource);
}


int cg_remove(resource_type t){

    int r = 0;
    for (int i = 0; i < CGROUP_CTR_NUM; i++) {
        int msk = 1 << i;
        if (t & msk) {
            if (cgctl.ss[i] == NULL) {
                continue;
            }
            r += subsystem_remove(cgctl.ss[i]);
            cgctl.ss[i] = NULL;
            cgctl.t &= ~msk;
        }

    }
    return r;

    // if (!(t & cgctl.t)){
    //     fprintf(stderr, "remove a invalid cgroup\n");
    //     return -1;
    // } 
    // cgctl.t &= ~t;
    // subsystem *ss = cgctl.ss[bit2num(t)];
    // if (ss && subsystem_remove(ss)){
    //     // failed
    //     free(ss);
    //     cgctl.ss[bit2num(t)] = NULL;
    //     return -1;
    // }
    // cgctl.ss[bit2num(t)] = NULL;
    // return 0;
}

int cg_add(resource_type t, pid_t pid){

    int r = 0;
    for (int i = 0; i < CGROUP_CTR_NUM; i++) {
        int msk = 1 << i;
        if (t & msk) {
            if (cgctl.ss[i] == NULL) {
                continue;
            }
            r += subsystem_add(cgctl.ss[i], pid);
        }
    }

    return r;
    // if (!(t & cgctl.t)){
    //     fprintf(stderr, "add a invalid cgroup\n");
    //     return -1;
    // } 
    // subsystem *ss = cgctl.ss[bit2num(t)]; 
    // return subsystem_add(ss, pid);
}

int cg_del(){
    for (int i = 0; i < CGROUP_CTR_NUM; i++) {
        if (cgctl.ss[i] != NULL) {
            cg_remove((1 << i));
        }
    }
    return 0;
}