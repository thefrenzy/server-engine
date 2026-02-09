#include "Logger.h"
#include <iostream>  // Added for std::cout
#include <vector>
#include <mutex>

std::vector<std::string> Logger::logs;
std::mutex Logger::logMutex;

void Logger::Log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    // 1. Storage for Graphical Interface
    logs.push_back(message);
    
    // 2. Output to the Terminal/Console
    std::cout << message << std::endl;

    // Maintain buffer size
    if (logs.size() > 500) {
        logs.erase(logs.begin());
    }
}

std::vector<std::string> Logger::GetLogs() {
    std::lock_guard<std::mutex> lock(logMutex);
    return logs;
}