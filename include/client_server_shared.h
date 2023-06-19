#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @struct server_info_t
 * @brief Server Address Info Type
 */
typedef struct server_info_s {
    struct sockaddr_in ip_addr;
    uint16_t app_port;
    uint16_t rank;
} server_info_t;

/**
 * @struct client_info_t
 * @brief Client Address Info Type
 */
typedef struct client_info_s {
    struct sockaddr_in ip_addr;
    uint16_t rank;
    int iterations;
} client_info_t;

inline server_info_t *parse_saddress_info(char *args) {
    // TODO: Not yet implemented
    return NULL;
}

inline client_info_t *parse_caddress_info(char *args) {
    // TODO: Not yet implemented
    return NULL;
}
