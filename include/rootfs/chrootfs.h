#ifndef LIGHTCONTAINER_CHROOTFS_H
#define LIGHTCONTAINER_CHROOTFS_H

#define _GUN_SOURCE
#define DEFAULT_NEW_ROOT_FS "/tmp/rootfs"

int chrootfs(char *nrfs);

#endif