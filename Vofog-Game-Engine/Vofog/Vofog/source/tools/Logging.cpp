
#include <vpch.h>
#include "Logging.h"

#include "spdlog/sinks/basic_file_sink.h" 
namespace Vofog {


	std::shared_ptr<Vofog::my_sink_mt> Logging::_guiSink = std::make_shared<Vofog::my_sink_mt>();
	std::shared_ptr<spdlog::logger> Logging::_coreLogger(new spdlog::logger("Vofog", { std::make_shared<spdlog::sinks::stdout_color_sink_mt>(), Logging::_guiSink }));
	std::shared_ptr<spdlog::logger> Logging::_clientLogger;

	void Logging::init(Vofog::ImGuiConsole* console) {
		//VOFOG_ASSERT(_imguiConsole != nullptr);
		_guiSink->_vfg_setGuiConsole(console);

		_coreLogger->set_pattern("%^[%T]%@ %n: %v%$");
		spdlog::set_pattern("%^[%T]%@ %n: %v%$");


		auto colorSink = std::make_shared < spdlog::sinks::stdout_color_sink_mt>();
		auto mySink = std::make_shared<Vofog::my_sink_mt>();

		_coreLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("Sandbox");
		_coreLogger->set_level(spdlog::level::trace);


	}




}