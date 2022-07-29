#pragma once

#if (defined _DEBUG == 1)
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#elif (defined FEDES_VERBOSE == 1 || defined FEDES_STATS == 1) 
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else 
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif 

// Log level must be defined prior to including spdlog.h
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/fmt/bundled/color.h>

#if (defined _DEBUG == 1 || defined FEDES_VERBOSE == 1)
	#define FEDES_TRACE(...) SPDLOG_TRACE( __VA_ARGS__)
	#define FEDES_DEBUG(...) SPDLOG_DEBUG( __VA_ARGS__)
	#define FEDES_INFO(...) SPDLOG_INFO( __VA_ARGS__)
	#define FEDES_STATS_LOG(...) SPDLOG_INFO( __VA_ARGS__)
	#define FEDES_WARN(...) SPDLOG_WARN( __VA_ARGS__)
	#define FEDES_ERROR(...) SPDLOG_ERROR( __VA_ARGS__)
	#define FEDES_CRITICAL(...) SPDLOG_CRITICAL( __VA_ARGS__)
#elif (defined FEDES_STATS == 1) 
	#define FEDES_DEBUG(...)
	#define FEDES_TRACE(...)
	#define FEDES_INFO(...)
	#define FEDES_STATS_LOG(...) SPDLOG_INFO( __VA_ARGS__)
	#define FEDES_WARN(...)
	#define FEDES_ERROR(...)
	#define FEDES_CRITICAL(...)
#else 
	#define FEDES_DEBUG(...)
	#define FEDES_TRACE(...)
	#define FEDES_INFO(...)
    #define FEDES_STATS_LOG(...)
	#define FEDES_WARN(...)
	#define FEDES_ERROR(...)
	#define FEDES_CRITICAL(...)
#endif 