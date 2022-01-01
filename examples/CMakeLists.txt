
add_executable( test_client test_client.cpp)
target_link_libraries(test_client PRIVATE rtsplib spdlog::spdlog)

add_executable( test_server test_server.cpp)
target_link_libraries(test_server PRIVATE rtsplib spdlog::spdlog)