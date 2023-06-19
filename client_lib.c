#include "client_server_shared.h"
#include "client_lib.h"
#include <time.h>

client_ctx_t *setup_client(struct sockaddr_in *client_addr,
			   struct sockaddr_in *server_addr) {

    int rc = -1;

    // Allocate a connection object
    client_ctx_t *ctx = calloc(1, sizeof(client_ctx_t));
    API_NULL(ctx,
	{
	    return (NULL);
	},
	"Unable to allocate client ctx obj\n");

    ctx->fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    API_STATUS(
        ctx->fd,
        {
            free(ctx);
            return (NULL);
        },
        "Unable to open SOCK_STREAM connection\n");

    // Bind a connection to an client IP address
    rc = bind(ctx->fd, (struct sockaddr *)(client_addr), sizeof(struct sockaddr_in));
    API_STATUS(
        rc,
        {
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to bind connection to IP address: %s\n",
	inet_ntoa((client_addr->sin_addr)));

    // Connect to server IP address
    rc = connect(ctx->fd, (struct sockaddr *)(server_addr),
		 sizeof(struct sockaddr_in));
    API_STATUS(
        rc,
        {
            close(ctx->fd);
            free(ctx);
            return (NULL);
        },
        "Unable to connect to IP address: %s\n",
	inet_ntoa((server_addr->sin_addr)));

    return (ctx);
}

ssize_t tx_client(void **buf) {
    ssize_t i = 0;
    ssize_t nbytes = 64;
    // Randomize Tx on client before send
    // Print Tx on client for debugging
    srand(time(NULL));
    for (i = 0; i < nbytes; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }

        *(uint8_t *)(buf[i]) = (rand() % 256);
        printf("%02x \t", *(uint8_t *)(buf[i]));
    }

    return (nbytes);
}

int rx_client(void **buf, ssize_t nbytes) {
    ssize_t i = 0;
    // Print Rx on client after recv
    // Randomize Tx on client before send
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
