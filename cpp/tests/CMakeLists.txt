find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(
    thrift-tests
    ${TEST_SOURCES}
)

target_link_libraries(
    thrift-tests
    PRIVATE
        thrift-playground
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(thrift-tests)

if(MSVC)
    target_compile_options(
        thrift-tests
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        thrift-tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
