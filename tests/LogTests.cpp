#include "gtest/gtest.h"

#include "../includes/Log.hpp"

TEST(Log, LogError) {
  testing::internal::CaptureStderr();
  nz::Log::error("Not really an error", "SOME_ERROR_TYPE", 42);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_STREQ(output.c_str(),
               "[ERROR 42] - SOME_ERROR_TYPE: Not really an error\n");
}

TEST(Log, LogInform) {
  testing::internal::CaptureStdout();
  nz::Log::inform("An informative message");
  std::string output = testing::internal::GetCapturedStdout();
  ASSERT_STREQ(output.c_str(), "[INFO]: An informative message\n");
}

TEST(Log, LogWarning) {
  testing::internal::CaptureStderr();
  nz::Log::warning("A warning message", "SOME_WARNING_TYPE");
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_STREQ(output.c_str(),
               "[WARNING] - SOME_WARNING_TYPE: A warning message\n");
}