cmake_minimum_required(VERSION 3.15)
project(CovertChannelsServer)
project(CovertChannelsClient)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lhiredis -luriparser")

add_executable(CovertChannelsClient tcpsocket.hpp tcpserver.hpp basesocket.hpp client.cpp ChatServer.cpp ChatServer.h ChatClient.cpp ChatClient.h)
add_executable(CovertChannelsServer tcpsocket.hpp tcpserver.hpp basesocket.hpp server.cpp ChatServer.cpp ChatServer.h ChatClient.cpp ChatClient.h)

target_link_libraries(CovertChannelsClient pthread)
target_link_libraries(CovertChannelsServer pthread)
#target_link_libraries(${PROJECT_NAME} hiredis uriparser tarantool)
