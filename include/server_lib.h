#ifndef SERVER_LIB_H
#define SERVER_LIB_H

#include <netinet/in.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * @struct server_ctx_t
 * @brief Server Connection Context Info
 */
typedef struct server_ctx_s {
    int fd;       //< Socket File Descriptor
    int epollfd;  //< Epoll fd instance
    void *msgbuf; //< Server-side message buffer
} server_ctx_t;

/**
 * @name API_STATUS_INTERNAL
 * @brief Convenience macro to check predicate, log error on console
 * and return
 */
#define API_STATUS_INTERNAL(expr, code_block, ...)                             \
    do {                                                                       \
        if (expr) {                                                            \
            printf(__VA_ARGS__);                                               \
            code_block;                                                        \
        }                                                                      \
    } while (0)

#define API_STATUS(rv, code_block, ...)                                        \
    API_STATUS_INTERNAL(((rv) < 0), code_block, __VA_ARGS__)

#define API_NULL(obj, code_block, ...)                                         \
    API_STATUS_INTERNAL(((obj) == NULL), code_block, __VA_ARGS__)

/**
 * @brief Given a user-defined IP and port, setup the server
 * control plane
 */
server_ctx_t *setup_server(struct sockaddr_in *addr, uint16_t port_id);

/**
 * @brief Given a user-defined msgbuf and nbytes, process the data
 * and produce outbut msgbuf (in-place) of the same size
 */
int process_server(void **buf, size_t nbytes);

/**
 * @brief Given a previously allocated server context info,
 * destroy the server control plane
 */
int destroy_server(server_ctx_t *ctx);

#endif /*! SERVER_LIB_H */
