#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

int __wrap_close(int fd)
{
    check_expected(fd);
    int ret = 0;
    return ret;
}

// Тестовый случай для проверки закрытия соединения с ненулевым указателем
static void test_close_connection_with_non_null_client(void **state)
{
    UnixClient client;
    client.client_fd = 1; // Пример ненулевого дескриптора сокета

    // Ожидаем, что close будет вызван с дескриптором client_fd
    expect_value(__wrap_close, fd, 1);
    // Возвращаем успешное закрытие соединения

    socket_error_t result = close_connection(&client);

    assert_int_equal(result, SOCKET_ERR_NONE); // Проверяем успешное завершение операции закрытия соединения
}

// Тестовый случай для проверки закрытия соединения с нулевым указателем
static void test_close_connection_with_null_client(void **state)
{
    UnixClient *client = NULL;

    socket_error_t result = close_connection(client);

    assert_int_equal(
            result,
            SOCKET_ERR_NULL_POINTER); // Проверяем, что функция возвращает ошибку, если передан нулевой указатель
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_close_connection_with_non_null_client),
        cmocka_unit_test(test_close_connection_with_null_client),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}