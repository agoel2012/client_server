#include "client_server_shared.h"
#include "server_lib.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ARGS 2

int make_fd_nonblocking(int fd) {
    int flags, rc;
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    rc = fcntl(fd, F_SETFL, flags);
    return (rc);
}

int start_server(server_info_t *sv) {

    struct epoll_event events = {0};
    int accept_fd = 0;
    ssize_t nbytes = 0;
    socklen_t addrlen = 0;
    int rc = 0;
    // Setup Server control plane
    server_ctx_t *ctx = setup_server(&sv->ip_addr, sv->app_port);
    API_NULL(
        ctx, { return (-1); }, "Server Setup Failed\n");
    bool server_stop = false;
    while (!server_stop) {
        rc = epoll_wait(ctx->epollfd, &events, 1, -1);
        API_STATUS(
            rc,
            {
                server_stop = true;
                break;
            },
            "Unable to epoll on listening sockets. Reason: %s\n",
            strerror(errno));

        if (events.data.fd == ctx->fd) {
            // Accept a new connection from client
            accept_fd =
                accept(ctx->fd, (struct sockaddr *)(&sv->peer_addr), &addrlen);
            events.data.fd = accept_fd;
            events.events = EPOLLIN | EPOLLOUT;
            make_fd_nonblocking(accept_fd);
            // Add it to the list of polling sockets
            epoll_ctl(ctx->epollfd, EPOLL_CTL_ADD, accept_fd, &events);
        } else {
            // Recv a message request
            nbytes = recv(events.data.fd, ctx->msgbuf, sizeof(msgbuf_t), 0);
            if (nbytes > 0) {
                // Send a message response
                rc = process_server(&ctx->msgbuf, nbytes);
                nbytes = send(events.data.fd, ctx->msgbuf, nbytes, 0);
            }
        }
    }

    // Teardown Server control plane
    // destroy_server();
    return (rc);
}

int main(int argc, char *argv[]) {
    if (argc < SERVER_ARGS) {
        printf("Usage: ./server <server IP:port>\n");
        return 1;
    }

    server_info_t *sv = parse_saddress_info(argv[1]);
    return (start_server(sv));
}
