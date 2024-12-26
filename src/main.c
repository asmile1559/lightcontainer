#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "cgroup/cgroup.h"

int main()
{   
    int r = init_cgroupctl(CGROUP_ALL);    
    if (r < 0){
        return -1;
    }

    pid_t pid = getpid();
    cg_set(CGROUP_CPU, "5000");
    cg_set(CGROUP_MEMORY, "536870912 ");
    cg_set(CGROUP_CPUSET, "0");
    cg_set(CGROUP_PID, "20");

    cg_add(CGROUP_CPU, pid);
    cg_add(CGROUP_MEMORY, pid);
    cg_add(CGROUP_CPUSET, pid);
    cg_add(CGROUP_PID, pid);

    // cg_del();
    return 0;
}