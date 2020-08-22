#include "hardwareinfo.h"
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>

void machine(char* result)
{
    if (result == (char*)NULL) {
        return;
    }
    struct utsname uts;
    if (uname(&uts) == -1) {
        return;
    }
    strcpy(result, uts.machine);
}
