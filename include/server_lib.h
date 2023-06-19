#ifndef SERVER_LIB_H
#define SERVER_LIB_H

#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

/**
 * @struct server_ctx_t
 * @brief Server Connection Context Info
 */
typedef struct server_ctx_s {
    int fd; //< Socket File Descriptor
            // TODO:
} server_ctx_t;

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
    } while (1)

#define API_STATUS(rv, code_block, ...)                                        \
    API_STATUS_INTERNAL(((rv) != 0), code_block, __VA_ARGS__)

#define API_NULL(obj, code_block, ...)                                         \
    API_STATUS_INTERNAL(((obj) == NULL), code_block, __VA_ARGS__)

/**
 * @brief Given a user-defined IP and port, setup the server
 * control plane
 */
server_ctx_t *setup_server(struct sockaddr_in *addr, uint16_t port_id);

/**
 * @brief Given a previously allocated server context info,
 * destroy the server control plane
 */
int destroy_server(server_ctx_t *ctx);

#endif /*! SERVER_LIB_H */
