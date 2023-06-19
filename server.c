#include "client_server_shared.h"
#include "server_lib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ARGS 2

int start_server(server_info_t *sv) {

    struct epoll_event events = {0};
    int rc = 0;
    ssize_t nbytes = 0;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    // Setup Server control plane
    server_ctx_t *ctx = setup_server(&sv->ip_addr, sv->app_port);
    API_NULL(
        ctx, { return (-1); }, "Server Setup Failed\n");
    bool server_stop = false;

    while (!server_stop) {
        // Accept a new connection from client
        rc = accept(ctx->fd, (struct sockaddr *)&sv->ip_addr, &addrlen);
        API_STATUS(
            rc,
            {
                server_stop = true;
                return (rc);
            },
            "Unable to accept new connection\n");

        // Recv a message request
        // Send a response
        rc = epoll_wait(ctx->epollfd, &events, 1, -1);
        if (rc == 1 && events.data.fd == ctx->fd) {
            nbytes = recv(ctx->fd, ctx->msgbuf, sizeof(msgbuf_t), 0);
            rc = process_server(&ctx->msgbuf, nbytes);
            nbytes = send(ctx->fd, ctx->msgbuf, nbytes, 0);
        }
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
