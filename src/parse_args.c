#include "parse_args.h"
#include <stdio.h>
#include <string.h>

void print_help(){
printf(
"Usage: lightcontainer [OPTIONS]\n\
    -h  Hostname of container, if not set, a random hostname will be given.\n\
    -n  New rootfs path, must be an ABSOLUTE path. Default is '/home/user/rootfs'\n\
    -o  Old rootfs path, must be a RELATIVE path from new rootfs path. Default is 'oldroot'.\n\
    -d  Veth id of container. Default is 2\n\
    -p  Veth ip of container. Default is '10.0.0.2/24'\n\
    -D  Veth id of host. Default is 1\n\
    -P  Veth ip of host. Default is '10.0.0.1/24'\n\
    -B  [OPTIONAL] Virtual bridge id, must be exist. Connect host veth to bridge(if given)\n\
    -c  CPU limit by cgroups. [0 ~ 100000 * cpu_count]. Default is 50000 (50%% in single core)\n\
    -m  Memory limit (in byte) by cgroups. [0 ~ host_memory]. Default is 5242880 (512MB)\n\
");
}

int parse_args(int argc, char **argv, lc_args *lca)
{

    memset(lca, 0, sizeof(lc_args));

    lca->nrfs = DEFAULT_NEW_ROOTFS;
    lca->orfs = DEFAULT_OLD_ROOTFS;
    lca->cvethid = DEFAULT_CVETHID;
    lca->cvethip = DEFAULT_CVETHIP;
    lca->hvethid = DEFAULT_HVETHID;
    lca->hvethip = DEFAULT_HVETHIP;
    lca->cpilimit = DEFAULT_CPU_LIMIT;
    lca->memlimit = DEFAULT_MEM_LIMIT;

    for (int i = 1;i < argc; i+=2){
        char *flag = argv[i];
        char *value = argv[i+1];

        switch (flag[1])
        {
        case 'h':
            lca->hostname = value;
            break;
        case 'n':
            lca->nrfs = value;
            break;
        case 'o':
            lca->orfs = value;
            break;
        case 'd':
            lca->cvethid = value;
            break;
        case 'p':
            lca->cvethip = value;
            break;
        case 'D':
            lca->hvethid = value;
            break;
        case 'P':
            lca->hvethip = value;
            break;
        case 'B':
            lca->hbrid = value;
            break;
        case 'c':
            lca->cpilimit = value;
            break;
        case 'm':
            lca->memlimit = value;
            break;
        default:
            fprintf(stderr, "invalid options -<%s>. \n", flag);
            print_help();
            break;
        }
    }

    return 0;
}
