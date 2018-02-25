#include "gtest/gtest.h"

#include "../src/zia.hh"

TEST(Zia, ZiaNotYetStarted) {
    nz::zia Zia;
    std::string output;

    // Zia not yet started, trying to reload
    testing::internal::CaptureStdout();
    Zia.reload();
    output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: Server is not start\n");
    // Zia not yet started, trying to stop
    testing::internal::CaptureStdout();
    Zia.stop();
    output = testing::internal::GetCapturedStdout();
    ASSERT_STREQ(output.c_str(), "[ERROR 1] - Zia Core: Server is not start\n");
}

TEST(Zia, ZiaNormalBehaviour) {
    nz::zia Zia;

    // Zia.start();
}