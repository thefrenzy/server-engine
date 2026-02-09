#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <mutex>

class Logger {
public:

    static void Log(const std::string& message);
    static std::vector<std::string> GetLogs();

private:
    static std::vector<std::string> logs;
    static std::mutex logMutex;
};

#endif