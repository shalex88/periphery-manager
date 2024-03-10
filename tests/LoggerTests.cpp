#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "Logger/Logger.h"

// TEST PLAN
// + Able to log
// + Not able to log messages with lower seeverity
// + Can change severity on runtime
// + Can log trace
// + Can log debug
// + Can log warning
// + Can log error
// + Can log critical

TEST(LoggerTests, AbleToOutput){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    std::string expected_output="Hello World!";
    LOG_INFO("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr(expected_output));
}

TEST(LoggerTests, NotAbleToOutputLowerSeverity){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    std::string expected_output="Hello World!";
    LOG_DEBUG("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), "");
}

TEST(LoggerTests, AbleToChangeSeverityOnRuntime){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    std::string expected_output="Hello World!";
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kDebug);
    LOG_DEBUG("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr(expected_output));
}

TEST(LoggerTests, AbleToLogTrace){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kTrace);
    std::string expected_output="Hello World!";
    LOG_TRACE("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("trace"));
}

TEST(LoggerTests, AbleToLogDebug){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kDebug);
    std::string expected_output="Hello World!";
    LOG_DEBUG("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("debug"));
}

TEST(LoggerTests, AbleToLogInfo){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kInfo);
    std::string expected_output="Hello World!";
    LOG_INFO("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("info"));
}

TEST(LoggerTests, AbleToLogWarning){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kWarn);
    std::string expected_output="Hello World!";
    LOG_WARN("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("warning"));
}

TEST(LoggerTests, AbleToLogError){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kError);
    std::string expected_output="Hello World!";
    LOG_ERROR("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("error"));
}

TEST(LoggerTests, AbleToLogCritical){
//    Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

//    Call the function you want to test
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kCritical);
    std::string expected_output="Hello World!";
    LOG_CRITICAL("{}", expected_output);

//    Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

//    Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("critical"));
}


TEST(LoggerTests, SettingWrongSeverityLevelThrowsException){
    auto wrong_severity = static_cast<LoggerInterface::LogLevel>(10);

    EXPECT_THROW(Logger::getInstance().setLogLevel(wrong_severity), std::invalid_argument);
}

TEST(LoggerTests, LogWrongSeverityThrowsException){
    auto wrong_severity = static_cast<LoggerInterface::LogLevel>(10);
    std::string expected_output="Hello World!";

    EXPECT_THROW(Logger::getInstance().log(wrong_severity,"{}", expected_output), std::invalid_argument);
}
