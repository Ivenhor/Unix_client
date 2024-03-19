cmake_minimum_required(VERSION 3.0.0)
project(Unix_client VERSION 0.1.0 LANGUAGES C)

enable_testing()

include(cmake/FetchCMocka.cmake)

add_library(Unix_socket unix_socket/unix_socket.c unix_socket/unix_socket.h)

add_executable(Unix_client main.c)

target_link_libraries(Unix_client
	PRIVATE
    Unix_socket
)

add_executable(unit_tests
	test/main.c
)

target_link_libraries(unit_tests PRIVATE cmocka-static Unix_socket)
add_test(NAME unit_tests COMMAND unit_tests)
set(CPACK_PACKAGE_NAME "Unix_socket")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
