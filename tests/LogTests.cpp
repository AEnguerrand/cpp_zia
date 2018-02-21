#include "gtest/gtest.h"

#include "../includes/Log.hpp"

TEST(Log, LogError) {
    nz::Log::error("Not really an error", "SOME_ERROR_TYPE", 42);
}

TEST(Log, LogInform) {
    nz::Log::inform("An informing message");
}