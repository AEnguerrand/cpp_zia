#include "gtest/gtest.h"

#include "../src/zia.hh"

TEST(Zia, ZiaNotYetStarted) {
    nz::zia Zia;
    std::string output;

    // Zia not yet started, trying to reload
    testing::internal::CaptureStderr();
    Zia.reload();
    output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: The server isn't started yet\n");
    // Zia not yet started, trying to stop
    testing::internal::CaptureStderr();
    Zia.stop();
    output = testing::internal::GetCapturedStderr();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: The server isn't started yet\n");
}

TEST(Zia, ZiaNormalBehaviour) {
    nz::zia Zia;

    // Zia.start();
}