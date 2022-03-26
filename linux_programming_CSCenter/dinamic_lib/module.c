#include <stdio.h>
#include "module.h"

void module_printf(const char *text) {
    printf("#| %s!\n", text);
    return;
};
