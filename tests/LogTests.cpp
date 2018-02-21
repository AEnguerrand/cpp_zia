#include "gtest/gtest.h"

#include "../includes/Log.hpp"

TEST(Log, LogError) {
    nz::Log::error("Not really an error", "SOME_ERROR_TYPE", 42);
}

TEST(Log, LogInform) {
    nz::Log::inform("An informing message");
}

TEST(Log, LogWarning) {
    nz::Log::warning("An warning message", "SOME_WARNING_TYPE");
}