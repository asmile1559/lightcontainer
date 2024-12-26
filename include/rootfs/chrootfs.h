#ifndef LIGHTCONTAINER_CHROOTFS_H
#define LIGHTCONTAINER_CHROOTFS_H

#define _GUN_SOURCE
#define DEFAULT_NEW_ROOT_FS "/tmp/rootfs"
#define OLDROOTFS "oldrootfs"
int chrootfs(char *nrfs, char *orfs);
#endif