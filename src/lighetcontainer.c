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


#define STACK_SIZE (1024 * 1024)
char stack[STACK_SIZE];

int lightcontainer(void *args)
{
    lc_opt *opt = args;
    if (opt == NULL){
        opt = (lc_opt *)malloc(sizeof(lc_opt));
        if (opt == NULL) {
            perror("malloc failed");
            return -1;
        }
        memset(opt, 0, sizeof(lc_opt));
    }

    if (opt->nrfs == NULL){
        opt->nrfs = DEFAULT_NEW_ROOT_FS;
    }
    if (opt->hostname == NULL) {
        opt->hostname = (char *)malloc(sizeof(char) * 9);
        sprintf(opt->hostname, "%08x", rand() % 1000000000);
    }
    if (opt->vethID == 0) {
        opt->vethID = DEFAULT_CONTAINER_VETHID;
    }

    if (opt->vethIP == NULL) {
        opt->vethIP = DEFAULT_CONTAINER_IP;
    }

    if (chrootfs(opt->nrfs, opt->orfs) < 0)
    {
        return -1;
    }

    if (sethostname(opt->hostname, strlen(opt->hostname)) < 0) 
    {
        perror("set host name error");
        return -1;
    }

    sleep(1);
    if (setup_veth(opt->vethIP, opt->vethID) < 0){
        perror("set veth error");
        return -1;
    }
    if (veth_up(opt->vethID) < 0) {
        perror("veth up error");
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