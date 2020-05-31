#pragma once
#include <Vofog.h>
#define SPDLOG_TRACE_ON
#include "spdlog/spdlog.h"
#include <ostream>
#include <sstream>
#include <iostream>
#include <strstream>
#include <spdlog/sinks/sink.h>
#include "spdlog/sinks/ostream_sink.h" 
#include "spdlog/sinks/stdout_color_sinks.h"
#include <tools/GuiSink.h>
#include <tools/ImGui/ImGuiConsole.h>

#include <mutex>


namespace Vofog {
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	class VOFOG_API Logging {

	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
		static std::shared_ptr<Vofog::GuiSink<std::mutex>> _guiSink;

	public:
		static void init(Vofog::ImGuiConsole* console = nullptr);


		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; };

	};



}





#ifdef VOFOG_DEBUG_BUILD

#define VOFOG_CORE_TRACE(...) ::Vofog::Logging::getCoreLogger()->trace(__VA_ARGS__)
#define VOFOG_CORE_DEBUG(...) ::Vofog::Logging::getCoreLogger()->debug(__VA_ARGS__)
#define VOFOG_CORE_LOG(...) ::Vofog::Logging::getCoreLogger()->info(__VA_ARGS__)
#define VOFOG_CORE_WARN(...) ::Vofog::Logging::getCoreLogger()->warn(__VA_ARGS__)
#define VOFOG_CORE_ERROR(...) ::Vofog::Logging::getCoreLogger()->error(__VA_ARGS__)
#define VOFOG_CORE_FATAL(...) ::Vofog::Logging::getCoreLogger()->critical(__VA_ARGS__)

#else 
#define VOFOG_CORE_TRACE(...)
#define VOFOG_CORE_DEBUG(...)
#define VOFOG_CORE_LOG(...) 
#define VOFOG_CORE_WARN(...)
#define VOFOG_CORE_ERROR(...) 
#define VOFOG_CORE_FATAL(...) 

#endif

#define VOFOG_TRACE(...) ::Vofog::Logging::getClientLogger()->trace(__VA_ARGS__)
#define VOFOG_DEBUG(...) ::Vofog::Logging::getClientLogger()->debug(__VA_ARGS__)
#define VOFOG_LOG(...) ::Vofog::Logging::getClientLogger()->info(__VA_ARGS__)
#define VOFOG_WARN(...) ::Vofog::Logging::getClientLogger()->warn(__VA_ARGS__)
#define VOFOG_ERROR(...) ::Vofog::Logging::getClientLogger()->error(__VA_ARGS__)
#define VOFOG_FATAL(...) ::Vofog::Logging::getClientLogger()->critical(__VA_ARGS__)