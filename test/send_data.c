#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

ssize_t __wrap_send(int sockfd, const void *buf, size_t len, int flags)
{
    // Проверяем переданные значения
    check_expected(sockfd);
    check_expected_ptr(buf);
    check_expected(len);
    check_expected(flags);

    // Здесь можно добавить дополнительную логику или просто вернуть значение из мока
    return mock_type(ssize_t);
}

// Тестовый случай для успешной отправки данных
static void test_send_data_success(void **state)
{
    UnixClient client;
    client.client_fd = 123; // Пример ненулевого дескриптора сокета
    strcpy(client.command_input, "test command"); // Пример команды для отправки

    // Ожидаем, что функция send будет вызвана с соответствующими параметрами
    expect_value(__wrap_send, sockfd, client.client_fd);
    expect_string(__wrap_send, buf, client.command_input);
    expect_value(__wrap_send, len, strlen(client.command_input));
    expect_value(__wrap_send, flags, 0);
    // Возвращаем успешную отправку данных
    will_return(__wrap_send, 0);

    // Вызываем функцию для тестирования
    socket_error_t result = send_data(&client);

    // Проверяем, что функция завершилась без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
}

// Тестовый случай для ошибки отправки данных
static void test_send_data_error(void **state)
{
    UnixClient client;
    client.client_fd = 123; // Пример ненулевого дескриптора сокета
    strcpy(client.command_input, "test command"); // Пример команды для отправки

    // Ожидаем, что функция send будет вызвана с соответствующими параметрами
    expect_value(__wrap_send, sockfd, client.client_fd);
    expect_string(__wrap_send, buf, client.command_input);
    expect_value(__wrap_send, len, strlen(client.command_input));
    expect_value(__wrap_send, flags, 0);
    // Возвращаем ошибку при отправке данных
    will_return(__wrap_send, -1);

    // Вызываем функцию для тестирования
    socket_error_t result = send_data(&client);

    // Проверяем, что функция вернула ошибку отправки
    assert_int_equal(result, SOCKET_ERR_SEND);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_send_data_success),
        cmocka_unit_test(test_send_data_error),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}