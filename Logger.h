#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

namespace Logger {
    void Log(const std::string& message);
    std::vector<std::string> GetLogs();
}

#endif