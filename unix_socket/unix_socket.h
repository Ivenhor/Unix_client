#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef enum {
    SOCKET_ERR_NONE,
    SOCKET_ERR_SOCKET,
    SOCKET_ERR_CONNECT,
    SOCKET_ERR_SEND,
    SOCKET_ERR_NULL_POINTER,
    SOCKET_ERR_RECV,
    SOCKET_ERR_INPUT
} socket_error_t;

typedef enum { COMMAND_DATE, COMMAND_TIME, COMMAND_EXIT, COMMAND_SHUTDOWN, COMMAND_UNKNOWN } client_command_t;

typedef struct {
    int                client_fd;
    client_command_t   command;
    char               command_input[100];
    char               response[100];
    struct sockaddr_un server_addr;
} UnixClient;

socket_error_t initialize_client(UnixClient *client, const char *socket_path);

socket_error_t connect_to_server(UnixClient *client);

socket_error_t send_data(UnixClient *client);

socket_error_t receive_data(UnixClient *client);

socket_error_t close_connection(UnixClient *client);

socket_error_t get_user_command(UnixClient *client);

#endif /*_UNIX_SOCKET_H_*/