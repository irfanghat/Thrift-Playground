cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

project(
    ThriftPlayground
    VERSION 1.0
    DESCRIPTION "Thrift playground..."
)

add_subdirectory(thrift)

add_executable(thrift_playground
    src/thrift_client.cpp
)

target_compile_features(
    thrift_playground
    PRIVATE
        cxx_std_20
)

target_link_libraries(
    thrift_playground
    PRIVATE
        thrift-playground
)

enable_testing()
add_subdirectory(tests)