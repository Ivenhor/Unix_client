#include "unix_socket.h"

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

socket_error_t close_connection(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }

    // Закрытие сокета клиента
    close(client->client_fd);

    return SOCKET_ERR_NONE;
}

__attribute__((weak)) int set_gpio(int pin)
{
    return pin;
}

socket_error_t mock_test(UnixClient *client)
{
    if (client == NULL) {
        return SOCKET_ERR_NULL_POINTER;
    }
    if (set_gpio(10) != 0) {
        return (SOCKET_ERR_INPUT);
    };

    return SOCKET_ERR_NONE;
}