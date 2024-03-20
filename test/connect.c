#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Мок для функции connect
int __wrap_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    // Проверяем переданные значения
    check_expected(sockfd);
    check_expected(addr);
    check_expected(addrlen);

    return mock_type(int);
}

// Тестовый случай для успешного подключения к серверу
static void test_connect_to_server_success(void **state)
{
    UnixClient client;
    client.client_fd = 123; // Пример ненулевого дескриптора сокета

    // Ожидаем, что функция connect будет вызвана с соответствующими параметрами
    expect_value(__wrap_connect, sockfd, client.client_fd);
    expect_any(__wrap_connect, addr); // Параметр addr может быть любым указателем
    expect_value(__wrap_connect, addrlen, sizeof(struct sockaddr_un));
    // Возвращаем успешное подключение к серверу
    will_return(__wrap_connect, 0);

    // Вызываем функцию для тестирования
    socket_error_t result = connect_to_server(&client);

    // Проверяем, что функция завершилась без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
}

// Тестовый случай для ошибки подключения к серверу
static void test_connect_to_server_error(void **state)
{
    UnixClient client;
    client.client_fd = 123; // Пример ненулевого дескриптора сокета

    // Ожидаем, что функция connect будет вызвана с соответствующими параметрами
    expect_value(__wrap_connect, sockfd, client.client_fd);
    expect_any(__wrap_connect, addr); // Параметр addr может быть любым указателем
    expect_value(__wrap_connect, addrlen, sizeof(struct sockaddr_un));
    // Возвращаем ошибку при подключении к серверу
    will_return(__wrap_connect, -1);

    // Вызываем функцию для тестирования
    socket_error_t result = connect_to_server(&client);

    // Проверяем, что функция вернула ошибку подключения
    assert_int_equal(result, SOCKET_ERR_CONNECT);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_connect_to_server_success),
        cmocka_unit_test(test_connect_to_server_error),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}