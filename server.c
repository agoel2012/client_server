#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "client_server_shared.h"

#define SERVER_ARGS 2

int start_server(server_info_t *sv) {

    // Setup Server control plane
    // setup_server();
    bool server_stop = false; // TODO: Use signal handler to terminate it

    while (!server_stop) {
	// Accept a new connection from client
	// Recv a message request
	// Send a response
    }

    // Teardown Server control plane
    // destroy_server();
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < SERVER_ARGS) {
        printf("Usage: ./server <server IP:port>\n");
        return 1;
    }

    server_info_t *sv = parse_saddress_info(argv[1]);
    return (start_server(sv));
}    
