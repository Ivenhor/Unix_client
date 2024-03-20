#include "unix_socket.h"

/**
 * @brief Initializes a UnixClient structure and sets up a Unix domain socket connection.
 *
 * This function initializes a UnixClient structure with the given socket path and sets up
 * a Unix domain socket connection. It creates a socket using the AF_UNIX domain and SOCK_STREAM type,
 * and then sets the server address to the provided socket path.
 *
 * @param client A pointer to the UnixClient structure to initialize.
 * @param socket_path The path to the Unix domain socket.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 *     - SOCKET_ERR_SOCKET if socket creation fails.
 */
socket_error_t initialize_client(UnixClient *client, const char *socket_path)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Создание сокета
    if ((client->client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return SOCKET_ERR_SOCKET;
    }

    // Установка адреса сервера
    memset(&client->server_addr, 0, sizeof(struct sockaddr_un));
    client->server_addr.sun_family = AF_UNIX;
    strncpy(client->server_addr.sun_path, socket_path, sizeof(client->server_addr.sun_path) - 1);

    return SOCKET_ERR_NONE;
}

/**
 * @brief Obtains a user command from standard input and sets it in the provided UnixClient structure.
 *
 * This function prompts the user to enter a command (date, time, exit, or shutdown) and reads
 * the input from standard input. It then removes the newline character from the input and determines
 * the corresponding command type, which is stored in the client structure.
 *
 * @param client A pointer to the UnixClient structure to store the user command.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 *     - SOCKET_ERR_INPUT if an error occurs while reading input from stdin.
 */
socket_error_t get_user_command(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Очистка массива command_input
    memset(client->command_input, 0, sizeof(client->command_input));

    printf("Enter command (date, time, exit или shutdown): ");
    fflush(stdout); // Очистка буфера вывода

    if (fgets(client->command_input, sizeof(client->command_input), stdin) == NULL) {
        perror("fgets");
        return SOCKET_ERR_INPUT;
    }

    // Удаление символа новой строки из ввода
    client->command_input[strcspn(client->command_input, "\n")] = 0;

    // Определение команды
    if (strcmp(client->command_input, "date") == 0) {
        client->command = COMMAND_DATE;
    } else if (strcmp(client->command_input, "time") == 0) {
        client->command = COMMAND_TIME;
    } else if (strcmp(client->command_input, "exit") == 0) {
        client->command = COMMAND_EXIT;
    } else if (strcmp(client->command_input, "shutdown") == 0) {
        client->command = COMMAND_SHUTDOWN;
    } else {
        client->command = COMMAND_UNKNOWN;
    }

    return SOCKET_ERR_NONE;
}

/**
 * @brief Connects the client to the server using the provided UnixClient structure.
 *
 * This function establishes a connection between the client and the server using the socket
 * file descriptor and server address stored in the UnixClient structure.
 *
 * @param client A pointer to the UnixClient structure containing the client's file descriptor and server address.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 *     - SOCKET_ERR_CONNECT if an error occurs while attempting to connect to the server.
 */
socket_error_t connect_to_server(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Подключение к серверу
    if (connect(client->client_fd, (struct sockaddr *)&client->server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        return SOCKET_ERR_CONNECT;
    }

    return SOCKET_ERR_NONE;
}

/**
 * @brief Sends data to the server using the provided UnixClient structure.
 *
 * This function sends the command input stored in the UnixClient structure to the server
 * using the client's file descriptor.
 *
 * @param client A pointer to the UnixClient structure containing the client's file descriptor and command input.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 *     - SOCKET_ERR_SEND if an error occurs while attempting to send data to the server.
 */
socket_error_t send_data(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Отправка команды серверу
    if (send(client->client_fd, client->command_input, strlen(client->command_input), 0) == -1) {
        perror("send");
        return SOCKET_ERR_SEND;
    }

    return SOCKET_ERR_NONE;
}

/**
 * @brief Receives data from the server using the provided UnixClient structure.
 *
 * This function receives data from the server using the client's file descriptor
 * and stores it in the response buffer of the UnixClient structure.
 *
 * @param client A pointer to the UnixClient structure containing the client's file descriptor and response buffer.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 *     - SOCKET_ERR_RECV if an error occurs while attempting to receive data from the server.
 */
socket_error_t receive_data(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Получение ответа от сервера
    ssize_t bytes_received = recv(client->client_fd, client->response, sizeof(client->response), 0);
    if (bytes_received == -1) {
        perror("recv");
        return SOCKET_ERR_RECV;
    }

    client->response[bytes_received] = '\0';

    printf("Server response: %s\n", client->response);

    return SOCKET_ERR_NONE;
}

/**
 * @brief Closes the connection to the server for the provided UnixClient structure.
 *
 * This function closes the connection by closing the client's file descriptor.
 *
 * @param client A pointer to the UnixClient structure containing the client's file descriptor.
 * @return A socket_error_t value indicating the result of the operation.
 *     - SOCKET_ERR_NONE if the operation is successful.
 *     - SOCKET_ERR_NULL_POINTER if the client pointer is NULL.
 */
socket_error_t close_connection(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Закрытие сокета клиента
    close(client->client_fd);

    return SOCKET_ERR_NONE;
}