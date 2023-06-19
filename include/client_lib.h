#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <netinet/in.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#define MAX_ITERATIONS 1000000ULL
#define SEC_TO_NSEC 1000000000ULL

/**
 * @struct client_ctx_t
 * @brief Client Connection Context Info
 */
typedef struct client_ctx_s {
    int fd;                           //< Socket File Descriptor
    void *msgbuf;                     //< Client-side message buffer
    uint64_t elapsed[MAX_ITERATIONS]; //< Latency tracking
    size_t cur_elapsed;               //< tracking pointer
} client_ctx_t;

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
 * @name Timers for client latency tracking
 */
#define CLIENT_DECLARATIONS()                                                  \
    struct timespec __start = {0};                                             \
    struct timespec __end = {0};

#define CLIENT_START_TIME() clock_gettime(CLOCK_MONOTONIC, &__start);

#define CLIENT_GET_ELAPSED_TIME(ctx)                                           \
    clock_gettime(CLOCK_MONOTONIC, &__end);                                    \
    ctx->elapsed[ctx->cur_elapsed++] =                                         \
        ((__end.tv_nsec + __end.tv_sec * SEC_TO_NSEC) -                        \
         (__start.tv_nsec + __start.tv_sec * SEC_TO_NSEC));

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

void print_client(client_ctx_t *ctx, uint16_t rank, int iterations,
                  long unsigned int xfer_sz);

#endif /*! CLIENT_LIB_H */
