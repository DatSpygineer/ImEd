#include "imed_common.hpp"

#include "ImEdNativeDiag.hpp"

#include <chrono>
#include <fmt/format.h>

#if !defined(IMED_CORE)

template<>
struct fmt::formatter<DebugMessageType> : public fmt::formatter<std::string_view> {
	auto format(DebugMessageType value, fmt::format_context& ctx) {
		std::string_view name;
		switch (value) {
			case DebugMessageType::Info: name = "Info"; break;
			case DebugMessageType::Warning: name = "Warning"; break;
			case DebugMessageType::Error: name = "Error"; break;
			case DebugMessageType::FatalError: name = "Fatal"; break;
		}
		return fmt::formatter<std::string_view>::format(name, ctx);
	}
};

void ImEdLogImpl(const std::string& message, DebugMessageType messageType, DebugInfo debugInfo) {
#ifdef NDEBUG
	if (messageType == DebugMessageType::Error || messageType == DebugMessageType::FatalError) {
#endif
	auto date = std::chrono::system_clock::now();
	auto fmsg = fmt::format("{} [{}] {}:{} {} - {}",
							date,
							messageType,
							debugInfo.callerFile, debugInfo.callerLine, debugInfo.callerFunction,
							message);
	printf("%s\n", fmsg.c_str());

#ifndef NDEBUG
	if (messageType == DebugMessageType::Error || messageType == DebugMessageType::FatalError) {
#endif
		MessageBox::Error("Error!", fmsg);
#ifndef NDEBUG
	}
#endif

	if (messageType == DebugMessageType::FatalError) {
		exit(1);
	}

#ifdef NDEBUG
	}
#endif
}
#endif