#include "client_lib.h"
#include "client_server_shared.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define CLIENT_ARGS 4

int start_client(client_info_t *cv, server_info_t *sv) {
    ssize_t nbytes = 0;
    int rc = -1;
    // Setup Client control plane and connect to Server
    client_ctx_t *ctx = setup_client(&(cv->ip_addr), &(sv->ip_addr));
    API_NULL(
        ctx, { return (rc); }, "Client Setup Failed\n");

    CLIENT_DECLARATIONS();
    for (int i = 0; i < cv->iterations; i++) {
        nbytes = tx_client(&(ctx->msgbuf));
        CLIENT_START_TIME();
        // Send a message request
        nbytes = send(ctx->fd, ctx->msgbuf, nbytes, 0);
        // Recv a message response
        nbytes = recv(ctx->fd, ctx->msgbuf, sizeof(msgbuf_t), 0);
        // Track until max iterations only
        if (i < MAX_ITERATIONS) {
            CLIENT_GET_ELAPSED_TIME(ctx);
        }

        rc = rx_client(&(ctx->msgbuf), nbytes);
    }

    // Print Statistics:
    print_client(ctx, cv->rank, cv->iterations, nbytes * cv->iterations);

    // Teardown Client control plane
    // destroy_client();
    return (rc);
}

int main(int argc, char *argv[]) {

    if (argc < CLIENT_ARGS) {
        printf("Usage: ./client <server IP:port> <client IP> <iterations>\n");
        return 1;
    }

    server_info_t *sv = parse_saddress_info(argv[1]);
    client_info_t *cv = parse_caddress_info(argv[2], argv[3]);
    start_client(cv, sv);
    return 0;
}
