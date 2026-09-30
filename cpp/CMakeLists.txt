cmake_minimum_required(VERSION 3.15)

project(
    ThriftPlayground
    VERSION 1.0
    DESCRIPTION "Thrift playground..."
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(thrift)

add_executable(
    thrift_client
    src/thrift_client.cpp
)

add_executable(
    thrift_server
    src/thrift_server.cpp
)

set_target_properties(
    thrift_client
    PROPERTIES
        LINKER_LANGUAGE CXX
)

target_link_libraries(
    thrift_client
    PRIVATE
        thrift-playground
)

target_link_libraries(
    thrift_server
    PRIVATE
        thrift-playground
)

enable_testing()
add_subdirectory(tests)