// Logger.cpp
#include "Logger.h"
#include <format>

std::vector<Message> Logger::messages;
std::mutex Logger::mutex;

void Logger::Log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::string prefix;
    Message msg;

    switch (level) {
    case LogLevel::INFO:    prefix = "[INFO] ";   msg.r = 200; msg.g = 200; msg.b = 200; msg.shell_color = "\033{37m"; break;
    case LogLevel::WARNING: prefix = "[WARN] ";   msg.r = 255; msg.g = 200; msg.b = 20;  msg.shell_color = "\033{33m"; break;
    case LogLevel::ERROR:   prefix = "[ERROR] ";  msg.r = 255; msg.g = 20;  msg.b = 20;  msg.shell_color = "\033{31m"; break;
    }

    std::string fullMessage = prefix + message;
    msg.msg = fullMessage;


    messages.push_back(msg);

    // Print to console

    // also handle console color

    // shell colors dont work with vs console
    //std::cout << msg.shell_color << fullMessage << std::endl;
    std::cout << fullMessage << std::endl;
}

void Logger::Spacer() {
    Logger::Log(LogLevel::INFO, "< - - - - - - - - - - - - - - - - - - - - - - >");
}