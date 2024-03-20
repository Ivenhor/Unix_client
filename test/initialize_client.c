#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Mock для функции socket
int __wrap_socket(int domain, int type, int protocol)
{
    check_expected(domain);
    check_expected(type);
    check_expected(protocol);
    return mock_type(int);
}

static void test_initialize_client_null_pointer(void **state)
{
    UnixClient *client      = NULL;
    const char *socket_path = "/tmp/socket_test";

    // Вызываем функцию для тестирования с нулевым указателем
    socket_error_t result = initialize_client(client, socket_path);

    // Проверяем, что функция возвращает ошибку нулевого указателя
    assert_int_equal(result, SOCKET_ERR_NULL_POINTER);
}

static void test_initialize_client_success(void **state)
{
    UnixClient  client;
    const char *socket_path = "/tmp/socket_test";

    // Ожидаем вызов socket с корректными параметрами
    expect_value(__wrap_socket, domain, AF_UNIX);
    expect_value(__wrap_socket, type, SOCK_STREAM);
    expect_value(__wrap_socket, protocol, 0);
    // Возвращаем успешное создание сокета
    will_return(__wrap_socket, 0);

    // Вызываем функцию для тестирования
    socket_error_t result = initialize_client(&client, socket_path);

    // Проверяем, что функция завершилась без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
    // Проверяем, что дескриптор сокета был корректно установлен
    assert_true(client.client_fd >= 0);
    // Проверяем, что адрес сервера был корректно установлен
    assert_string_equal(client.server_addr.sun_path, socket_path);
}

static void test_initialize_client_socket_failure(void **state)
{
    UnixClient  client;
    const char *socket_path = "/tmp/socket_test";

    // Ожидаем вызов socket с корректными параметрами
    expect_value(__wrap_socket, domain, AF_UNIX);
    expect_value(__wrap_socket, type, SOCK_STREAM);
    expect_value(__wrap_socket, protocol, 0);
    // Возвращаем ошибку при создании сокета
    will_return(__wrap_socket, -1);

    // Вызываем функцию для тестирования
    socket_error_t result = initialize_client(&client, socket_path);

    // Проверяем, что функция возвращает ошибку создания сокета
    assert_int_equal(result, SOCKET_ERR_SOCKET);
    // Проверяем, что дескриптор сокета не был установлен
    assert_int_equal(client.client_fd, -1);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_initialize_client_null_pointer),
        cmocka_unit_test(test_initialize_client_success),
        cmocka_unit_test(test_initialize_client_socket_failure),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}