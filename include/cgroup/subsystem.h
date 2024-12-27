#ifndef LIGHTCONTAINER_SUBSYSTEM_H
#define LIGHTCONTAINER_SUBSYSTEM_H

#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>

#include "types.h"

#define CPU_CFS_QUOTA_US "cpu.cfs_quota_us"
#define CPU_CFS_PERIOD_US "cpu.cfs_period_us"
#define DEFAULT_CPU_CFS_PERIOD_US "100000"

#define CPUSET_CPUS "cpuset.cpus"

#define MEMORY_LIMIT_IN_BYTES "memory.limit_in_bytes"

#define BLKIO_THROTTLE_READ_BPS_DEVICE "blkio.throttle.read_bps_device"
#define BLKIO_THROTTLE_WRITE_BPS_DEVICE "blkio.throttle.write_bps_device"

#define PIDS_MAX "pids.max"

typedef struct{
    resource_type t;
    int _;
    char *type_name;
    char *ctl_root;
    char *ctl_file;
    char *tasks_file;
    char *procs_file;
}subsystem_t;

// create subsystem
subsystem_t *init_subsystem(resource_type t, char *container_id);

// 
int subsystem_set(subsystem_t *ss, char *resource);
int subsystem_remove(subsystem_t *ss) ;
int subsystem_add(subsystem_t *ss, pid_t pid);

#endif