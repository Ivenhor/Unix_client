#include <stdio.h>
#include "unix_socket/unix_socket.h"

#define SOCKET_PATH "/tmp/socket_test"

int main()
{
    UnixClient     client;
    socket_error_t err;

    // Инициализация клиента
    err = initialize_client(&client, SOCKET_PATH);
    if (err != SOCKET_ERR_NONE) {
        fprintf(stderr, "Ошибка инициализации клиента.\n");
        exit(EXIT_FAILURE);
    }

    // Подключение к серверу
    err = connect_to_server(&client);
    if (err != SOCKET_ERR_NONE) {
        fprintf(stderr, "Ошибка подключения к серверу.\n");
        exit(EXIT_FAILURE);
    }

    printf("Введите команду (date, time, exit или shutdown): ");
    while (1) {

        // Получение команды от пользователя
        err = get_user_command(&client);
        if (err != SOCKET_ERR_NONE) {
            fprintf(stderr, "Ошибка при получении команды.\n");
            break;
        }

        // Отправка команды серверу
        err = send_data(&client);
        if (err != SOCKET_ERR_NONE) {
            fprintf(stderr, "Ошибка при отправке данных.\n");
            break;
        }

        // Получение ответа от сервера
        err = receive_data(&client);
        if (err != SOCKET_ERR_NONE) {
            fprintf(stderr, "Ошибка при получении ответа от сервера.\n");
            break;
        }

        // Если введена команда exit, завершаем работу
        if (client.command == COMMAND_EXIT || client.command == COMMAND_SHUTDOWN) {
            printf("Отправлена команда выхода. Завершение работы.\n");
            break;
        }
    }

    // Закрытие сокета клиента
    close(client.client_fd);

    return 0;
}
