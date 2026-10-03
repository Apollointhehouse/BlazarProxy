#include <stdio.h>
#include <stdlib.h>

#include "../include/proxy.h"

#define PORT 25565

int32_t main(int32_t argc, char const* argv[])
{
    proxy(PORT);

    printf("Proxy Stopped\n");

    exit(EXIT_SUCCESS);
}