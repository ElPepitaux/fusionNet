/**
 * @file standard_logger.hpp
 * @brief Standard output/error logger declaration.
 *
 * Declares `utility::StandardLogger`, which routes INFO_LEVEL/DEBUG_LEVEL to
 * stdout and WARNING_LEVEL/ERROR_LEVEL to stderr. See implementation in
 * sources/standard_logger.cpp.
 */

#pragma once

#include "common/Platform.hpp"

namespace fus::logging
{

	/**
	 * @brief Standard output/error logger implementation.
	 *
	 * Logs DEBUG_LEVEL and INFO_LEVEL messages to stdout, WARNING_LEVEL and
	 * ERROR_LEVEL to stderr.
	 */
	class StandardLogger
	{

		/**
		 * @brief Log severity levels.
		 */
		enum class LogLevel {
			DEBUG_LEVEL,	  ///< Detailed debugging information
			INFO_LEVEL,		  ///< General informational messages
			WARNING_LEVEL,	  ///< Warning messages for potentially harmful
							///< situations
			ERROR_LEVEL		  ///< Error messages for serious problems
		};

		public:
		/**
		 * @brief Constructor with logger name.
		 * @param name The name of the logger.
		 */
		StandardLogger(const std::string &name);

		/**
		 * @brief Destructor ensuring output streams are flushed.
		 */
		~StandardLogger(void);

		/**
		 * @brief Log a debug message to stdout.
		 * @param message The message to log.
		 */
		static void debug(const std::string &message);

		/**
		 * @brief Log an informational message to stdout.
		 * @param message The message to log.
		 */
		static void info(const std::string &message);

		/**
		 * @brief Log a warning message to stderr.
		 * @param message The message to log.
		 */
		static void warning(const std::string &message);

		/**
		 * @brief Log an error message to stderr.
		 * @param message The message to log.
		 */
		static void error(const std::string &message);

		/**
		 * @brief Log a message with specified level.
		 *
		 * Routes DEBUG_LEVEL and INFO_LEVEL to stdout, WARNING_LEVEL and
		 * ERROR_LEVEL to stderr.
		 * @param level The severity level.
		 * @param message The message to log.
		 */
		static void log(LogLevel level, const std::string &message);

		/**
		 * @brief Get string representation of log level.
		 * @param level The log level to convert.
		 * @return String representation of the level.
		 */
		static std::string levelToString(LogLevel level);

		/**
		 * @brief Get current timestamp as formatted string.
		 * @return Formatted timestamp string.
		 */
		static std::string getTimestamp(void);

		/**
		 * @brief Format a log message with timestamp, logger name, and level.
		 * @param level The log level.
		 * @param message The message to format.
		 * @return Formatted log message.
		 */
		static std::string formatMessage(LogLevel level,
								  const std::string &message);
	};

}	 // namespace utility::logging
