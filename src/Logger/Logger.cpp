#include <ctime>

#include "./Logger.h"

void Logger::log(string message) {
    if (!debug) return;

    time_t timestamp = time(nullptr);
    string timeString = asctime(localtime(&timestamp));
    timeString.pop_back();
    string pid = to_string(getpid());
    string logMessage = "pid: " + pid + " (" + timeString + ") " + message + "\n";

    // escribir(logMessage.c_str(), logMessage.length());

}

Logger::Logger(string output, bool debug) :
        debug(debug) {}

Logger::~Logger() = default;