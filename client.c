#include "client_server_shared.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define CLIENT_ARGS 3

int start_client(client_info_t *cv, server_info_t *sv) {

    // Setup Client control plane
    // setup_client();

    // Connect a new connection to a server
    for (int i = 0; i < cv->iterations; i++) {
        // Send a message request
        // Recv a response
    }

    // Teardown Client control plane
    // destroy_client();
    return 0;
}

int main(int argc, char *argv[]) {

    if (argc < CLIENT_ARGS) {
        printf("Usage: ./client <server IP:port> <client IP>\n");
        return 1;
    }

    server_info_t *sv = parse_saddress_info(argv[1]);
    client_info_t *cv = parse_caddress_info(argv[2]);
    start_client(cv, sv);
    return 0;
}
