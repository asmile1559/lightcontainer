#define _GUN_SOURCE
#include <rootfs/chrootfs.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int chrootfs(char *nrfs, char *orfs){

    if (nrfs == NULL) {
        nrfs = DEFAULT_NEW_ROOT_FS;
    }
    
    char oldroot[128];
    sprintf(oldroot, "%s/%s", nrfs, orfs);
    // printf("%s\n", oldroot);
    struct stat s; 
    if((stat(oldroot, &s) != 0) && mkdir(oldroot, 0777) < 0){
        perror("make old root fs error");
        return -1;
    }

    if (mount(NULL, "/", NULL, MS_PRIVATE | MS_REC, NULL) < 0) {
        perror("private old rootfs error");
        return -1;
    }

    if (mount(nrfs, nrfs, NULL, MS_BIND | MS_REC, NULL) < 0) {
        perror("mount new rootfs error");
        return -1;
    }

    if (pivot_root(nrfs, oldroot) < 0) {
        printf("nrfs: %s\norfs: %s\n", nrfs, orfs);
        perror("pivot_root error");
        return -1;
    }
    
    chdir("/");

    if (umount2(orfs, MNT_DETACH) < 0){
        perror("umount2 error");
        return -1;
    }

    if (rmdir(orfs) < 0){
        perror("rmdir error");
        return -1;
    }

    if ((stat("/proc", &s) != 0) && mkdir("/proc", 0777) < 0){
        perror("mkdir /proc error");
        return -1;
    }
    if ((stat("/sys", &s) != 0) && mkdir("/sys", 0777) < 0){
        perror("mkdir /sys error");
        return -1;
    }
    
    if (mount("proc", "/proc", "proc", 0, NULL) < 0) {
        perror("mount /proc error");
        return -1;
    }

    if (mount("sysfs", "/sys", "sysfs", 0, NULL) < 0) {
        perror("mount /sys error");
        return -1;
    }

    return 0;
}