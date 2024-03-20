#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

// Мок для функции fgets
char *__wrap_fgets(char *s, int size, FILE *stream)
{
    // Проверяем переданные значения
    check_expected(s);
    check_expected(size);
    check_expected_ptr(stream);
    char *mocked_value = mock_ptr_type(char *);

    if (mocked_value == NULL) {
        return NULL;
    } else {
        // Копируем значение в буфер s
        strncpy(s, mocked_value, size - 1);
        s[size - 1] = '\0';
    }
    return s;
}

// Тестовый случай для успешного получения команды от пользователя
static void test_get_user_command_success(void **state)
{
    UnixClient client;

    // Ожидаем, что функция fgets будет вызвана с соответствующими параметрами
    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream); // Параметр stream может быть любым указателем
    // Возвращаем успешное получение команды
    will_return(__wrap_fgets, "date");

    // Вызываем функцию для тестирования
    socket_error_t result = get_user_command(&client);

    // Проверяем, что функция завершилась без ошибок
    assert_int_equal(result, SOCKET_ERR_NONE);
    // Проверяем, что команда правильно определена
    assert_int_equal(client.command, COMMAND_DATE);
}

// Тест для команды "time"
static void test_get_user_command_time(void **state)
{
    UnixClient client;

    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream);
    will_return(__wrap_fgets, "time");

    socket_error_t result = get_user_command(&client);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(client.command, COMMAND_TIME);
}

// Тест для команды "exit"
static void test_get_user_command_exit(void **state)
{
    UnixClient client;

    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream);
    will_return(__wrap_fgets, "exit");

    socket_error_t result = get_user_command(&client);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(client.command, COMMAND_EXIT);
}

// Тест для команды "shutdown"
static void test_get_user_command_shutdown(void **state)
{
    UnixClient client;

    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream);
    will_return(__wrap_fgets, "shutdown");

    socket_error_t result = get_user_command(&client);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(client.command, COMMAND_SHUTDOWN);
}

// Тест для неизвестной команды
static void test_get_user_command_unknown(void **state)
{
    UnixClient client;

    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream);
    will_return(__wrap_fgets, "unknown_command");

    socket_error_t result = get_user_command(&client);

    assert_int_equal(result, SOCKET_ERR_NONE);
    assert_int_equal(client.command, COMMAND_UNKNOWN);
}

// Тестовый случай для ошибки ввода команды от пользователя
static void test_get_user_command_input_error(void **state)
{
    UnixClient client;

    // Ожидаем, что функция fgets будет вызвана с соответствующими параметрами
    expect_value(__wrap_fgets, s, client.command_input);
    expect_value(__wrap_fgets, size, sizeof(client.command_input));
    expect_any(__wrap_fgets, stream); // Параметр stream может быть любым указателем
    // Возвращаем ошибку ввода команды
    will_return(__wrap_fgets, NULL);

    // Вызываем функцию для тестирования
    socket_error_t result = get_user_command(&client);

    // Проверяем, что функция вернула ошибку ввода
    assert_int_equal(result, SOCKET_ERR_INPUT);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_user_command_success),  cmocka_unit_test(test_get_user_command_input_error),
        cmocka_unit_test(test_get_user_command_time),     cmocka_unit_test(test_get_user_command_exit),
        cmocka_unit_test(test_get_user_command_shutdown), cmocka_unit_test(test_get_user_command_unknown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}