cmake_minimum_required(VERSION 3.0.0)
project(Unix_client VERSION 0.1.0 LANGUAGES C)
include(CTest)
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
#add_test(NAME unit_tests COMMAND unit_tests)
set_target_properties(unit_tests PROPERTIES LINK_FLAGS "-Wl,--wrap=set_gpio")

##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(close_connection test/close_connection.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(close_connection PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME close_connection COMMAND close_connection)

set_target_properties(close_connection PROPERTIES LINK_FLAGS "-Wl,--wrap=close")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(receive_data test/receive_data.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(receive_data PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME receive_data COMMAND receive_data)
set_target_properties(receive_data PROPERTIES LINK_FLAGS "-Wl,--wrap=recv")

##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(send_data test/send_data.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(send_data PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME send_data COMMAND send_data)
set_target_properties(send_data PROPERTIES LINK_FLAGS "-Wl,--wrap=send")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(connect test/connect.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(connect PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME connect COMMAND connect)
set_target_properties(connect PROPERTIES LINK_FLAGS "-Wl,--wrap=connect")
##################################################################################
# Добавляем исполняемый файл для тестов закрытия соединения
add_executable(user_command test/user_command.c)

# Линкуем тесты с библиотекой Unix_socket и cmocka
target_link_libraries(user_command PRIVATE cmocka-static Unix_socket)

# Добавляем тесты закрытия соединения в список тестов
add_test(NAME user_command COMMAND user_command)
set_target_properties(user_command PROPERTIES LINK_FLAGS "-Wl,--wrap=fgets")
##################################################################################
set(CPACK_PACKAGE_NAME "Unix_socket")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
