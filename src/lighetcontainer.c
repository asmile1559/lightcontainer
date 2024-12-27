#include "cgroup/cgroup.h"
#include "rootfs/chrootfs.h"
#include "network/network.h"
#include "lightcontainer.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define STACK_SIZE (1024 * 1024)
char stack[STACK_SIZE];

int handle_container_network(veth_t *v, int *retry){
    int r;
    if (retry == NULL) {
        r = 5;
    } else {
        r = *retry;
    }
    
    while (r >= 0){
        sleep(1);
        r--;
        if (setup_veth(v) == 0){
            break;
        }
        fprintf(stderr, "remain retry times: %d", r);
        perror("");
    }

    if (r == 0) {
        printf(stderr, "handle_container_network error");
        perror("");
        return -1;
    }

    if (veth_up(v) < 0) {
        perror("veth up error");
        return -1;
    }
}

int lightcontainer(void *args)
{
    container_t *inst = args;

    if (inst == NULL){
        inst = (container_t *)malloc(sizeof(container_t));
        if (inst == NULL) {
            perror("malloc failed");
            return -1;
        }
        memset(inst, 0, sizeof(container_t));
    }

    if (inst->nrfs == NULL){
        inst->nrfs = DEFAULT_NEW_ROOT_FS;
    }

    if (inst->hostname == NULL) {
        inst->hostname = inst->container_id;
    }

    if (inst->veth.id == NULL) {
        inst->veth.id = DEFAULT_CONTAINER_VETHID;
    }

    if (inst->veth.ip == NULL) {
        inst->veth.ip = DEFAULT_CONTAINER_IP;
    }
    // char abs_oldroot[128];
    // sprintf(abs_oldroot, "%s/%s", inst->nrfs, inst->orfs);
    if (chrootfs(inst->nrfs, inst->orfs) < 0) {
        return -1;
    }

    if (sethostname(inst->hostname, strlen(inst->hostname)) < 0) {
        perror("set host name error");
        return -1;
    }

    if (handle_container_network(&inst->veth, NULL) < 0){
        return -1;
    }
   
    char *exargs[] = {"ls", NULL};
    if (execv("/bin/bash", exargs) < 0) {
        perror("execv error");
        return -1;
    } 
    return 0;
}

pid_t run_container(void *args)
{
    pid_t pid;
    char *stacktop = stack + STACK_SIZE;
    int flags = SIGCHLD|CLONE_NEWUTS|CLONE_NEWNS|CLONE_NEWNET|CLONE_NEWPID;
    pid = clone(lightcontainer, stacktop, flags, args);
    return pid;    
}