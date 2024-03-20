#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include "../unix_socket/unix_socket.h"

int set_gpio(int pin)
{
    check_expected(pin);
    printf("Value from mock %d", pin);
    if (pin != 10) {
        return -1;
    }
    return 0;
}

static void test_mock_test(void **state)
{
    UnixClient client;
    expect_value(set_gpio, pin, 10);
    socket_error_t result = mock_test(&client);
    assert_int_equal(result, SOCKET_ERR_NONE);
}

static void test_mock_test_error(void **state)
{
    UnixClient client;
    expect_value(set_gpio, pin, 10);
    socket_error_t result = mock_test(&client);
    assert_int_equal(result, SOCKET_ERR_INPUT);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mock_test),
        cmocka_unit_test(test_mock_test_error),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}