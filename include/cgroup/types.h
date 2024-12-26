#ifndef LIGHTCONTAINER_CGROUP_TYPES_H
#define LIGHTCONTAINER_CGROUP_TYPES_H

#include <stdio.h>
#include <unistd.h>
typedef int resource_type;
#define CGROUP_CTR_NUM 6

#define CGROUP_CPU 0x1
#define CGROUP_CPUSET 0x2
#define CGROUP_MEMORY 0x4
#define CGROUP_PID 0x8
#define CGROUP_BLKIN 0x10
#define CGROUP_BLKOUT 0x20
#define CGROUP_ALL (CGROUP_CPU|CGROUP_CPUSET|CGROUP_MEMORY|CGROUP_PID|CGROUP_BLKIN|CGROUP_BLKOUT)
#define CGROUP_BLKIO (CGROUP_BLKIN|CGROUP_BLKOUT)

static inline int bit2num(int bit){
    switch (bit)
    {
    case CGROUP_CPU:
        return 0;
    case CGROUP_CPUSET:
        return 1;
    case CGROUP_MEMORY:
        return 2;
    case CGROUP_PID:
        return 3;
    case CGROUP_BLKIN:
        return 4;
    case CGROUP_BLKOUT:
        return 5;
    default:
        fprintf(stderr, "error bit input\n");
        _exit(-1);
    }
}

#endif