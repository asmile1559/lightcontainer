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
    char *cgroup_root_path;
    resource_type t;
    subsystem *ss[CGROUP_CTR_NUM];
}cgroupctl;


extern cgroupctl cgctl;

int init_cgroupctl(resource_type t);
int cg_set(resource_type t, char *resource);
int cg_remove(resource_type t);
int cg_add(resource_type t, pid_t pid);
int cg_del();

#endif