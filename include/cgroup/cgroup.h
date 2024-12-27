#ifndef LIGHTCONTAINER_CGROUP_H
#define LIGHTCONTAINER_CGROUP_H

#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>

#include "types.h"
#include "subsystem.h"

#define CGROUP_ROOT_PATH "/sys/fs/cgroup"
#define CGROUP_GROUP_NAME "lightcontainer"

typedef struct {
    char *cpu_lim;
    char *mem_lim;
    char *cpuset_lim;
    char *blkin_lim;
    char *blkout_lim;
    char *pid_lim;
}cg_lim_t;

typedef struct {
    char *cgroup_root_path;
    resource_type t;
    subsystem_t *ss[CGROUP_CTR_NUM];
}cgroupctl_t;


extern cgroupctl_t cgctl;

int init_cgroupctl(resource_type t, char *container_id);
int cg_set(resource_type t, char *resource);
int cg_remove(resource_type t);
int cg_add(resource_type t, pid_t pid);
int cg_del();

#endif