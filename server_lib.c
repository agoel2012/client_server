#include "server_lib.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PENDING_CONNECTIONS 64

// Assumes SOCK_STREAM
server_ctx_t *setup_server(struct sockaddr_in *addr, uint16_t port_id) {
    int rc = 0;
    server_ctx_t *ctx = calloc(1, sizeof(server_ctx_t));
    // Allocate
    API_NULL(
        ctx, { return (NULL); }, "Unable to allocate server context\n");
    // open a connection
    ctx->fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    API_STATUS(
        ctx->fd,
        {
            free(ctx);
            return (NULL);
        },
        "Unable to open SOCK_STREAM connection\n");
    // bind a connection to an IP address
    rc = bind(ctx->fd, (struct sockaddr *)(addr), sizeof(struct sockaddr_in));
    API_STATUS(
        rc,
        {
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to bind connection to IP address\n");
    // listen for incoming requests on a connection
    rc = listen(ctx->fd, MAX_PENDING_CONNECTIONS);
    API_STATUS(
        rc,
        {
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to listen for incoming connections\n");

    return (ctx);
}
