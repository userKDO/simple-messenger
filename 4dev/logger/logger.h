#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>

class SimpleLogger
{
	private: 
		std::ofstream logFile;

		std::mutex logMutex;

		bool writeToConsole;

		std::string getCurrentTime()
		{
			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);
			std::tm tmBuf{}; // Buffer to store structured time
			localtime_r(&time, &tmBuf);
		

			std::ostringstream oss; // thread for creating string (like td::cout but into string)

			oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S");
			return oss.str();
		}

	public:
		SimpleLogger(const std::string& filename, bool consoleOutput = true)
			: writeToConsole(consoleOutput)
		{
			logFile.open(filename, std::ios::app);
			if (!logFile.is_open())
			{
				std::cerr << "Error: cannot open log file" << filename << std::endl;
			}
		}

		~SimpleLogger()
		{
			if (logFile.is_open()) 
			{
				logFile.close();
			}
		}

	void log(const std::string& message)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		std::string logEntry = "[" + getCurrentTime() + "] " + message;

		if (logFile.is_open())
		{
			logFile << logEntry << std::endl;
			logFile.flush();
		}

		if (writeToConsole)
		{
			std::cout << logEntry << std::endl;
		}
	}

	void info(const std::string& message)
	{
		log("[INFO] " + message);
	}

	void warning(const std::string& message)
	{
		log("[WARNING] " + message);
	}

	void error(const std::string& message)
	{
		log("[ERROR] " + message);
	}

	bool isOpen() const
	{
		return logFile.is_open();
	}
};

#endif
