#include "common/Logger.hpp"

namespace fus::logging
{

	StandardLogger::StandardLogger(const std::string &name)
	{
	}

	StandardLogger::~StandardLogger()
	{
		std::cout.flush();
		std::cerr.flush();
	}

	void StandardLogger::debug(const std::string &message)
	{
		log(LogLevel::DEBUG_LEVEL, message);
	}

	void StandardLogger::info(const std::string &message)
	{
		log(LogLevel::INFO_LEVEL, message);
	}

	void StandardLogger::warning(const std::string &message)
	{
		log(LogLevel::WARNING_LEVEL, message);
	}

	void StandardLogger::error(const std::string &message)
	{
		log(LogLevel::ERROR_LEVEL, message);
	}

	void StandardLogger::log(LogLevel level, const std::string &message)
	{
		std::string formatted = formatMessage(level, message);

		if (level == LogLevel::WARNING_LEVEL
			|| level == LogLevel::ERROR_LEVEL) {
			std::cerr << formatted << std::endl;
		} else {
			std::cout << formatted << std::endl;
		}
	}

		std::string StandardLogger::levelToString(LogLevel level)
	{
		switch (level) {
			case LogLevel::DEBUG_LEVEL:
				return "Debug";
			case LogLevel::INFO_LEVEL:
				return "Info";
			case LogLevel::WARNING_LEVEL:
				return "Warning";
			case LogLevel::ERROR_LEVEL:
				return "Error";
			default:
				return "Unknown";
		}
	}

	std::string StandardLogger::getTimestamp()
	{
		auto now  = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		auto ms	  = std::chrono::duration_cast<std::chrono::milliseconds>(
						now.time_since_epoch())
			% 1000;

		std::stringstream ss;
		ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
		return ss.str();
	}

	std::string StandardLogger::formatMessage(LogLevel level,
									  const std::string &message)
	{
		std::stringstream ss;
		ss << "[" << getTimestamp() << "] ";
		ss << "[" << levelToString(level) << "] " << message;
		return ss.str();
	}

}	 // namespace utility::logging
