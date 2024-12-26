#include "cgroup/cgroup.h"
#include "cgroup/subsystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

subsystem *init_subsystem(resource_type t)
{
    char *name;
    char *ctlfile;
    switch (t)
    {
    case CGROUP_CPU:
        name = "cpu";
        ctlfile = CPU_CFS_QUOTA_US;
        break;
    case CGROUP_CPUSET:
        name = "cpuset";
        ctlfile = CPUSET_CPUS;
        break;
    case CGROUP_MEMORY:
        name = "memory";
        ctlfile = MEMORY_LIMIT_IN_BYTES;
        break;
    case CGROUP_PID:
        name = "pids";
        ctlfile = PIDS_MAX;
        break;
    case CGROUP_BLKIN:
        name = "blkio";
        ctlfile = BLKIO_THROTTLE_READ_BPS_DEVICE;
        break;
    case CGROUP_BLKOUT:
        name = "blkio";
        ctlfile = BLKIO_THROTTLE_WRITE_BPS_DEVICE;
        break;
    default:
        fprintf(stderr, "invalid resource type\n");
        break;
    }

    char errbuf[100];
    char dirpath[256];
    struct stat s;
    sprintf(dirpath, "%s/%s/"CGROUP_GROUP_NAME, cgctl.cgroup_root_path, name);
    printf("%s\n", dirpath);
    if ((stat(dirpath, &s) != 0) && mkdir(dirpath, 0755) < 0) {
        sprintf(errbuf, "subsystem %s init error.", name);
        perror(errbuf);
        return NULL;
    }

    subsystem *ss = (subsystem *)malloc(sizeof(subsystem));
    ss->t = t;
    ss->name = name;
    ss->ctlfile = ctlfile;
    if (t == CGROUP_CPU) {
        sprintf(dirpath, "%s/cpu/"CGROUP_GROUP_NAME"/%s", cgctl.cgroup_root_path,  CPU_CFS_PERIOD_US);
        int fd = open(dirpath, O_WRONLY|O_TRUNC);
        if (fd < 0) {
            sprintf(errbuf, "subsystem %s init error.", ss->name);
            perror(errbuf);
            return NULL;
        }

        if (write(fd, DEFAULT_CPU_CFS_QUOTA_US, strlen(DEFAULT_CPU_CFS_QUOTA_US)) < 0){
            sprintf(errbuf, "subsystem %s init error.", ss->name);
            perror(errbuf);
            return NULL;
        }
    }
    return ss;
}

int subsystem_set(subsystem *ss, char *resource)
{
    char errbuf[100];
    char filepath[256];
    sprintf(filepath, "%s/%s/"CGROUP_GROUP_NAME"/%s", cgctl.cgroup_root_path, ss->name, ss->ctlfile);
    int fd = open(filepath, O_WRONLY|O_TRUNC);
    if (fd < 0) {
        sprintf(errbuf, "subsystem %s set error.", ss->name);
        perror(errbuf);
        return -1;
    }

    if (write(fd, resource, strlen(resource)) < 0){
        sprintf(errbuf, "subsystem %s set error.", ss->name);
        perror(errbuf);
        return -1;
    }
    close(fd);
    return 0;
}

int subsystem_add(subsystem *ss, pid_t pid) 
{
    char errbuf[100];
    char filepath[256];
    sprintf(filepath, "%s/%s/"CGROUP_GROUP_NAME"/tasks", cgctl.cgroup_root_path, ss->name);
    int fd = open(filepath, O_WRONLY|O_APPEND);
    if (fd < 0) {
        sprintf(errbuf, "subsystem %s add error.", ss->name);
        perror(errbuf);
        close(fd);
        return -1;
    }

    char s_pid[6];
    int n = sprintf(s_pid, "%d", pid);
    if (write(fd, s_pid, n) < 0){
        sprintf(errbuf, "%d, subsystem %s add error.", __LINE__,ss->name);
        perror(errbuf);
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int subsystem_remove(subsystem *ss) 
{
    char errbuf[100];
    char filepath[300];
    char dirpath[256];
    char cmd[300];
    sprintf(dirpath, "%s/%s/"CGROUP_GROUP_NAME, cgctl.cgroup_root_path, ss->name);
    sprintf(filepath, "%s/cgroup.procs", dirpath);

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("open file error");
        return -1;
    }
    
    while (!feof(file)) {
        char *line = NULL; 
        size_t len = 0;
        ssize_t read;
        memset(cmd, 0, 300);
        read = getline(&line, &len, file);
        if (read != -1) {
            if (line[read - 1] == '\n'){
                line[read - 1] = '\0';
            }
            read--;
            sprintf(cmd, "bash -c 'echo %s | tee /sys/fs/cgroup/%s/cgroup.procs'", line, ss->name);
            if (system(cmd) < 0){
                sprintf(errbuf, "subsystem %s remove error.", ss->name);
                perror(errbuf);
                return -1;
            }
        } else {
            free(line);
            break;
        }
        free(line); 
    }
    
    fclose(file);
    memset(cmd, 0, 300);
    sprintf(cmd, "rmdir %s", dirpath);
    if (system(cmd) < 0){
        sprintf(errbuf, "subsystem %s remove error.", ss->name);
        perror(errbuf);
        return -1;
    }

    free(ss);
    ss = NULL;
    return 0;
}