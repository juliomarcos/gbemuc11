#ifndef GBEMU_LOGGER
#define GBEMU_LOGGER

#include <cstdio>

namespace gbemu {
	
	enum class LogLevel {
		DEBUG = 0,
		INFO = 1,
		ERROR = 2
	};
	
	class Log {
		static LogLevel logLevel;
	public: 
		static void d(const char *format, ...) {
			if (logLevel > LogLevel::DEBUG) return;
			va_list arg;
			va_start (arg, format);
			vprintf(format, arg);
			va_end (arg);
		}
		static void i(const char *format, ...) {
			if (logLevel > LogLevel::INFO) return;
			va_list arg;
			va_start (arg, format);
			vprintf(format, arg);
			va_end (arg);
		}
		static void e(const char *format, ...) {
			va_list arg;
			va_start (arg, format);
			vprintf(format, arg);
			va_end (arg);
		}
		Log();
		virtual ~Log();
	};
	
	LogLevel Log::logLevel = LogLevel::DEBUG;
	
}

#endif