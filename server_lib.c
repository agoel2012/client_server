#include "server_lib.h"
#include "client_server_shared.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_PENDING_CONNECTIONS 64

// Assumes SOCK_STREAM
server_ctx_t *setup_server(struct sockaddr_in *addr, uint16_t port_id) {
    int rc = 0;
    struct epoll_event ev = {0};
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
        "Unable to bind connection to IP address: %s\n",
	inet_ntoa((addr->sin_addr)));
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

    ctx->epollfd = epoll_create(1);
    API_STATUS(
        ctx->epollfd,
        {
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to create epoll instance\n");

    ev.events = EPOLLIN;
    ev.data.fd = ctx->fd;
    rc = epoll_ctl(ctx->epollfd, EPOLL_CTL_ADD, ctx->fd, &ev);
    API_STATUS(
        rc,
        {
            close(ctx->epollfd);
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to change epoll ctl to monitor SOCK_STREAM\n");

    ctx->msgbuf = (void *)calloc(1, sizeof(msgbuf_t));
    API_NULL(
        ctx->msgbuf,
        {
            close(ctx->epollfd);
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to allocate server tx/rx app msg buf\n");

    return (ctx);
}

int process_server(void **buf, size_t nbytes) {
    size_t i = 0;
    // Print Rx on server after recv
    // Randomize Tx on server before send
    srand(time(NULL));
    for (i = 0; i < nbytes; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }

        printf("%02x \t", *(uint8_t *)(buf[i]));
        *(uint8_t *)(buf[i]) = (rand() % 256);
    }

    return (0);
}
