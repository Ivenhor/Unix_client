#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

ssize_t __wrap_recv(int sockfd, void *buf, size_t len, int flags)
{
    // Проверяем, что передан ожидаемый сокет
    check_expected(sockfd);
    // Проверяем, что указатель на буфер не равен NULL
    check_expected_ptr(buf);
    // Проверяем, что ожидается конкретная длина буфера
    check_expected(len);
    // Проверяем, что ожидаемые флаги совпадают с переданными
    check_expected(flags);

    printf("Received sockfd: %d\n", sockfd);
    printf("Received len: %zu\n", len);
    printf("Received flags: %d\n", flags);

    // Возвращаем количество принятых байт, определенное моком
    return mock_type(ssize_t);
}

// Тестовый случай для успешного получения данных от сервера
static void test_receive_data_success(void **state)
{
    UnixClient client;
    ssize_t    expected_bytes_received = 10; // Ожидаемое количество принятых байт

    // Устанавливаем дескриптор сокета в структуре клиента
    client.client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Устанавливаем ожидаемые параметры вызова recv
    expect_value(__wrap_recv, sockfd, client.client_fd);
    expect_any(__wrap_recv, buf);        // Ожидаем не NULL указатель на буфер
    expect_value(__wrap_recv, len, 100); // Ожидаемая длина буфера
    expect_value(__wrap_recv, flags, 0); // Ожидаемые флаги

    // Устанавливаем, что recv должен вернуть количество принятых байт, равное длине переданных данных
    will_return(__wrap_recv, expected_bytes_received);

    // Вызываем функцию для тестирования
    socket_error_t result = receive_data(&client);

    // Проверяем результат функции
    assert_int_equal(result, SOCKET_ERR_NONE); // Проверяем успешность операции
}

// Тестовый случай для ошибки при получении данных от сервера
static void test_receive_data_error(void **state)
{
    UnixClient client;
    ssize_t    expected_bytes_received = -1; // Ожидаемое количество принятых байт

    // Устанавливаем дескриптор сокета в структуре клиента
    client.client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Устанавливаем ожидаемые параметры вызова recv
    expect_value(__wrap_recv, sockfd, client.client_fd);
    expect_any(__wrap_recv, buf);        // Ожидаем не NULL указатель на буфер
    expect_value(__wrap_recv, len, 100); // Ожидаемая длина буфера
    expect_value(__wrap_recv, flags, 0); // Ожидаемые флаги

    // Устанавливаем, что recv должен вернуть количество принятых байт, равное длине переданных данных
    will_return(__wrap_recv, expected_bytes_received);

    // Вызываем функцию для тестирования
    socket_error_t result = receive_data(&client);

    // Проверяем результат функции
    assert_int_equal(result, SOCKET_ERR_RECV); // Проверяем, что получили ошибку при приеме данных
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_receive_data_success),
        cmocka_unit_test(test_receive_data_error),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}