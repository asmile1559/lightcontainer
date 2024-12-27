#include "cgroup/cgroup.h"
#include "cgroup/subsystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void free_subsystem(subsystem_t *ss){

    free(ss->ctl_root);
    free(ss->ctl_file);
    free(ss->tasks_file);
    free(ss->procs_file);
    free(ss);
    ss = NULL;
    return;
}

subsystem_t *init_subsystem(resource_type t, char *container_id)
{
    char *type_name;
    char *ctlfile;
    char errbuf[100];
    switch (t)
    {
    case CGROUP_CPU:
        type_name = "cpu";
        ctlfile = CPU_CFS_QUOTA_US;
        break;
    case CGROUP_CPUSET:
        type_name = "cpuset";
        ctlfile = CPUSET_CPUS;
        break;
    case CGROUP_MEMORY:
        type_name = "memory";
        ctlfile = MEMORY_LIMIT_IN_BYTES;
        break;
    case CGROUP_PID:
        type_name = "pids";
        ctlfile = PIDS_MAX;
        break;
    case CGROUP_BLKIN:
        type_name = "blkio";
        ctlfile = BLKIO_THROTTLE_READ_BPS_DEVICE;
        break;
    case CGROUP_BLKOUT:
        type_name = "blkio";
        ctlfile = BLKIO_THROTTLE_WRITE_BPS_DEVICE;
        break;
    default:
        fprintf(stderr, "invalid resource type\n");
        break;
    }

    subsystem_t *ss = (subsystem_t *)malloc(sizeof(subsystem_t));
    ss->t = t;
    ss->type_name = type_name;
    ss->ctl_root = (char *)malloc(sizeof(char) * 128);
    ss->ctl_file = (char *)malloc(sizeof(char) * 192);
    ss->tasks_file = (char *)malloc(sizeof(char) * 172);
    ss->procs_file = (char *)malloc(sizeof(char) * 172);

    sprintf(ss->ctl_root, "%s/%s/"CGROUP_GROUP_NAME"/%s", cgctl.cgroup_root_path, type_name, container_id);
    sprintf(ss->ctl_file, "%s/%s", ss->ctl_root, ctlfile);
    sprintf(ss->tasks_file, "%s/tasks", ss->ctl_root);
    sprintf(ss->procs_file, "%s/cgroup.procs", ss->ctl_root);

    // printf("ctl_root: %s\nctl_file: %s\ntasks: %s\nprocs: %s\n", ss->ctl_root, ss->ctl_file, ss->tasks_file, ss->procs_file);
    struct stat s;
    char cmd[256];
    sprintf(cmd, "mkdir -p %s", ss->ctl_root);
    if ((stat(ss->ctl_root, &s) != 0) && system(cmd) < 0) {
        sprintf(errbuf, "subsystem %s init error.", type_name);
        perror(errbuf);
        return NULL;
    }

    if (t == CGROUP_CPU) {
        char dirpath[256];
        sprintf(dirpath, "%s/%s", ss->ctl_root, CPU_CFS_PERIOD_US);
        int fd = open(dirpath, O_WRONLY|O_TRUNC);
        if (fd < 0) {
            sprintf(errbuf, "subsystem %s init error.", ss->type_name);
            perror(errbuf);
            free_subsystem(ss);
            return NULL;
        }

        if (write(fd, DEFAULT_CPU_CFS_PERIOD_US, strlen(DEFAULT_CPU_CFS_PERIOD_US)) < 0){
            sprintf(errbuf, "subsystem %s init error.", ss->type_name);
            perror(errbuf);
            free_subsystem(ss);
            return NULL;
        }
    }
    return ss;
}

int subsystem_set(subsystem_t *ss, char *resource)
{
    char errbuf[100];
    int fd = open(ss->ctl_file, O_WRONLY|O_TRUNC);
    if (fd < 0) {
        sprintf(errbuf, "subsystem %s set error.", ss->type_name);
        perror(errbuf);
        return -1;
    }

    if (write(fd, resource, strlen(resource)) < 0){
        sprintf(errbuf, "subsystem %s set error.", ss->type_name);
        perror(errbuf);
        return -1;
    }
    close(fd);
    return 0;
}

int subsystem_add(subsystem_t *ss, pid_t pid) 
{
    char errbuf[100];
    int fd = open(ss->tasks_file, O_WRONLY|O_APPEND);
    if (fd < 0) {
        sprintf(errbuf, "subsystem %s add error.", ss->type_name);
        perror(errbuf);
        close(fd);
        return -1;
    }

    char s_pid[6];
    int n = sprintf(s_pid, "%d", pid);
    if (write(fd, s_pid, n) < 0){
        sprintf(errbuf, "%d, subsystem %s add error.", __LINE__,ss->type_name);
        perror(errbuf);
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int subsystem_remove(subsystem_t *ss) 
{
    char errbuf[100];
    char cmd[300];

    FILE *file = fopen(ss->procs_file, "r");
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
            sprintf(cmd, "bash -c 'echo %s | tee /sys/fs/cgroup/%s/cgroup.procs'", line, ss->type_name);
            if (system(cmd) < 0){
                sprintf(errbuf, "subsystem %s remove error.", ss->type_name);
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
    sprintf(cmd, "rmdir %s", ss->ctl_root);
    if (system(cmd) < 0){
        sprintf(errbuf, "subsystem %s remove error.", ss->type_name);
        perror(errbuf);
        return -1;
    }

    free(ss);
    ss = NULL;
    return 0;
}