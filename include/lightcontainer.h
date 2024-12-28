#ifndef LIGHTCONTAINER_H
#define LIGHTCONTAINER_H

#define _GNU_SOURCE
#include <sys/types.h>
#include "network/network.h"

typedef struct 
{
    char container_id[16];
    char *hostname;
    char *nrfs;
    char *orfs;
    veth_t veth;
}container_t;

pid_t run_container(void *args);

#endif