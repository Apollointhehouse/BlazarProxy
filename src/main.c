#include <stdio.h>
#include <stdlib.h>

#include "proxy.h"
#include "util/logging.h"

#define PORT 25565

int32_t main(int32_t argc, char const* argv[])
{
    LOG_INFO("Starting Proxy!");

    proxy(PORT);

    LOG_INFO("Proxy Stopped");

    exit(EXIT_SUCCESS);
}