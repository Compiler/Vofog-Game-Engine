#pragma once
#include <Vofog.h>
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include <tools/GuiSink.h>
#include <mutex>
#include <tools/ImGui/ImGuiConsole.h>

namespace Vofog{
	template<typename Mutex>
	class VOFOG_API GuiSink: public spdlog::sinks::base_sink <Mutex>{
	public:
		void _vfg_setGuiConsole(Vofog::ImGuiConsole* console){ _imguiConsole = console; }

	protected:
		Vofog::ImGuiConsole* _imguiConsole;
		void sink_it_(const spdlog::details::log_msg& msg) override{

			// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
			// msg.raw contains pre formatted log

			// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			//std::cout << fmt::to_string(formatted);
			
			//VOFOG_ASSERT(_imguiConsole != nullptr);
			_imguiConsole->setlevel(msg.level);
			_imguiConsole->AddLog(fmt::to_string(formatted).c_str());
			
		}

		void flush_() override{

			std::cout << std::flush;
		}
	};


	using my_sink_mt = GuiSink<std::mutex>;
	using my_sink_st = GuiSink<spdlog::details::null_mutex>;

}