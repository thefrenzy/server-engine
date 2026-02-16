#include "Logger.h"
#include <iostream>
#include <vector>
#include <mutex>

static std::vector<std::string> g_logs;
static std::mutex g_logMutex;

void Logger::Log(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    
    g_logs.push_back(message);
    std::cout << message << std::endl;

    if (g_logs.size() > 500) {
        g_logs.erase(g_logs.begin());
    }
}

std::vector<std::string> Logger::GetLogs() {
    std::lock_guard<std::mutex> lock(g_logMutex);
    return g_logs;
}