#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "cgroup/cgroup.h"
#include "rootfs/chrootfs.h"
int main()
{   
    chrootfs("/home/user/rootfs");
    system("/bin/bash");
    return 0;
}