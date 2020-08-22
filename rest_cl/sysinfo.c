#include "sysinfo.h"
#include <sys/sysinfo.h>

size_t ramTotal()
{
    struct sysinfo info;
    sysinfo(&info);
    return info.totalram;
}

size_t ramFree()
{
    struct sysinfo info;
    sysinfo(&info);
    return info.freeram;
}

int procsNumber()
{
    return get_nprocs();
}
