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


// typedef struct {
//     char *vethID;
//     char *nrfs;
//     char *orfs;
//     char *hostname;
//     char *vethIP;
// }lc_opt;

pid_t run_container(void *args);

#endif