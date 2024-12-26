#include <stdio.h>
#include <stdlib.h>
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
#include "lightcontainer.h"
int main()
{   
    pid_t pid;
    lc_opt opt; 
    opt.hostname = "hi-lightcontainer";
    opt.nrfs = "/home/user/rootfs";
    opt.orfs = "/home/user/rootfs/"OLDROOTFS;
    opt.vethID = 12;
    opt.vethIP = "10.0.0.2/24";
   
    
    if ((pid = run_container((void *)&opt)) < 0){
        perror("run container error");
        _exit(-1);
    }

    if (create_veth_pair_mv2ns(11, 12, pid) < 0) {
        perror("create veth and move ns error");
        _exit(-1);
    }

    if (setup_veth("10.0.0.1/24", 11) < 0) {
        perror("setup veth error");
        _exit(-1);
    }

    if (veth_up(11) < 0) {
        perror("veth up error");
        _exit(-1);
    }
    
    init_cgroupctl(CGROUP_CPU|CGROUP_MEMORY);
    cg_set(CGROUP_CPU, "50000"); // 50% in single core
    cg_set(CGROUP_MEMORY, "5242880"); // 512MB
    cg_add(CGROUP_CPU|CGROUP_MEMORY, pid);

    waitpid(pid, NULL, 0);
    cg_remove(CGROUP_ALL);
 
    rmdir(opt.orfs);
    umount(opt.nrfs);
    umount("/");
    return 0;
}