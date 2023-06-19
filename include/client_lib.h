#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <netinet/in.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * @struct client_ctx_t
 * @brief Client Connection Context Info
 */
typedef struct client_ctx_s {
    int fd;       //< Socket File Descriptor
    void *msgbuf; //< Client-side message buffer
} client_ctx_t;

/**
 * @name API_STATUS_INTERNAL
 * @brief Convenience macro to check predicate, log error on console
 * and return
 */
#define API_STATUS_INTERNAL(expr, code_block, ...)                             \
    do {                                                                       \
        if (expr) {                                                            \
            code_block;                                                        \
            printf(__VA_ARGS__);                                               \
        }                                                                      \
    } while (0)

#define API_STATUS(rv, code_block, ...)                                        \
    API_STATUS_INTERNAL(((rv) < 0), code_block, __VA_ARGS__)

#define API_NULL(obj, code_block, ...)                                         \
    API_STATUS_INTERNAL(((obj) == NULL), code_block, __VA_ARGS__)

/**
 * @brief Given a user-defined client IP and server IP, setup the client
 * control plane
 */
client_ctx_t *setup_client(struct sockaddr_in *, struct sockaddr_in *);

/**
 * @brief Given a user-defined msgbuf, process the data
 * and produce outbut msgbuf (in-place) and return size as output arg 
 */
ssize_t tx_client(void **buf);
int rx_client(void **buf, ssize_t nbytes);

/**
 * @brief Given a previously allocated client context info,
 * destroy the client control plane
 */
int destroy_client(client_ctx_t *ctx);

#endif /*! CLIENT_LIB_H */
