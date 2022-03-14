cmake_minimum_required(VERSION 3.15)
project(CovertChannels)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lhiredis -luriparser")

file(GLOB sources "*.hpp" "*.cpp" "*.h")

add_executable(${PROJECT_NAME} ${sources} main.cpp)

target_link_libraries(${PROJECT_NAME} pthread)
#target_link_libraries(${PROJECT_NAME} hiredis uriparser tarantool)
