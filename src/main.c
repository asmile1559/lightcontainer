#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include "cgroup/cgroup.h"
#include "rootfs/chrootfs.h"
#include "network/network.h"
#include "parse_args.h"
#include "lightcontainer.h"

int handle_host_network(veth_t *v, char *brid) {
    if (setup_veth(v) < 0) {
        perror("setup veth error");
        return -1;
    }

    if (veth_up(v) < 0) {
        perror("veth up error");
        return -1;
    }

    if (brid != NULL && (link_veth_br(v, brid) < 0)){
        perror("link_veth_br error");
        return -1;
    }
    return 0;
}

int handle_cgroup(int flags, pid_t pid, cg_lim_t *cl) {
    if (flags & CGROUP_CPU){
        cg_set(CGROUP_CPU, cl->cpu_lim); 
    }

    if (flags & CGROUP_MEMORY){
        cg_set(CGROUP_MEMORY, cl->mem_lim); 
    }    

    cg_add(flags, pid);
}

int main(int argc, char *argv[])
{   
    srand(time(NULL));
    lc_args lca;
    pid_t pid;
    container_t inst; 
    veth_t hveth;
    cg_lim_t cl;
    int flags = CGROUP_CPU|CGROUP_MEMORY;
    char abs_oldroot[128];

    parse_args(argc, argv, &lca);
    memset(&inst, 0, sizeof(container_t));
    sprintf(inst.container_id, "%08x", rand() % 1000000000);
    sprintf(abs_oldroot, "%s/%s", lca.nrfs, lca.orfs);

    inst.hostname = lca.hostname;
    inst.nrfs = lca.nrfs;
    inst.orfs = lca.orfs;
    inst.veth.id = lca.cvethid;
    inst.veth.ip = lca.cvethip;
    inst.veth.state = STATE_INIT;

    hveth.id = lca.hvethid;
    hveth.ip = lca.hvethip;
    hveth.state = STATE_INIT;

    cl.cpu_lim = lca.cpilimit;
    cl.mem_lim = lca.memlimit;
    
    if ((pid = run_container((void *)&inst)) < 0){
        perror("run container error");
        _exit(-1);
    }

    if (create_veth_pair_mv2ns(&hveth, &inst.veth, pid) < 0) {
        perror("create veth and move ns error");
        _exit(-1);
    }
    
    if (handle_host_network(&hveth, lca.hbrid) < 0) {
        fprintf(stderr, "handle_host_network error\n");
        _exit(-1);
    }

    init_cgroupctl(flags, inst.container_id);
    handle_cgroup(flags, pid, &cl);
    waitpid(pid, NULL, 0);
    cg_remove(CGROUP_ALL);
 
    rmdir(abs_oldroot);
    umount(lca.nrfs);
    umount("/");
    return 0;
}