cmake_minimum_required(VERSION 3.16)

project(ClientServer)

include_directories(include)
add_compile_options(-g -O3 -Werror -Wall)
add_executable(Client client.c)
add_executable(Server server.c server_lib.c)

