#pragma once

#include <cstdio>

namespace gbemu {
	
	enum class LogLevel {
		DEBUG = 0,
		INFO = 1,
		ERROR = 2
	};
	
	class Log {
	public: 
		static LogLevel currentLogLevel;
		static void d(const char *format, ...) {
			if (currentLogLevel > LogLevel::DEBUG) return;
			va_list arg;
			va_start (arg, format);
			vprintf(format, arg);
			va_end (arg);
		}
		static void i(const char *format, ...) {
			if (currentLogLevel > LogLevel::INFO) return;
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
	
}