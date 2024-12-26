#ifndef LIGHTCONTAINER_H
#define LIGHTCONTAINER_H

#define _GNU_SOURCE
#include <sys/types.h>

typedef struct {
    int vethID;
    int _;
    char *nrfs;
    char *orfs;
    char *hostname;
    char *vethIP;
}lc_opt;

pid_t run_container(void *args);

#endif