#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "calculator_types.h"

using namespace calculator;

TEST(Thrift, SimpleImport)
{
    ASSERT_EQ(1 + 1, 2);
}