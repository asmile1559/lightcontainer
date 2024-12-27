#ifndef LIGHTCONTAINER_PARSE_ARGS_H
#define LIGHTCONTAINER_PARSE_ARGS_H

#define ARG_NUMS 10

// hostname
#define ARG_HOSTNAME "-h"

// rootfs
#define ARG_NEWROOTFS "-n"
#define ARG_OLDROOTFS "-o"

// network
#define ARG_CONTAINER_VETHID "-d"
#define ARG_CONTAINER_VETHIP "-p"
#define ARG_HOST_VETHID "-D"
#define ARG_HOST_VETHIP "-P"
#define ARG_HOST_BRIDGEID "-B"

// cgroup
#define ARG_CGROUP_CPU_LIMIT "-c"
#define ARG_CGROUP_MEM_LIMIT "-m"

#define DEFAULT_NEW_ROOTFS "/home/user/rootfs"
#define DEFAULT_OLD_ROOTFS "oldroot"
#define DEFAULT_CVETHID "2"
#define DEFAULT_CVETHIP "10.0.0.2/24"
#define DEFAULT_HVETHID "1"
#define DEFAULT_HVETHIP "10.0.0.1/24"
#define DEFAULT_CPU_LIMIT "50000"
#define DEFAULT_MEM_LIMIT "5242880"

typedef struct 
{
    char *hostname;
    char *nrfs;
    char *orfs; // relative to nrfs
    char *cvethid;
    char *cvethip;
    char *hvethid;
    char *hvethip;
    char *hbrid;
    char *cpilimit;
    char *memlimit;
    
}lc_args;

int parse_args(int argc, char **argv, lc_args *lca);
void print_help();
#endif