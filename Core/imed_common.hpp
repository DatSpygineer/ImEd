#pragma once

#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <fmt/format.h>

template<typename T>
concept IntegerType = std::is_integral_v<T>;
template<typename T>
concept FloatType = std::is_floating_point_v<T>;
template<typename T>
concept NumericType = std::is_integral_v<T> || std::is_floating_point_v<T>;

enum class DebugMessageType {
	Info, Warning, Error, FatalError
};

struct DebugInfo {
	const char* callerFile;
	const char* callerFunction;
	size_t callerLine;

	DebugInfo(const char* callerFile, const char* callerFunction, size_t callerLine):
		callerFile(callerFile), callerFunction(callerFunction), callerLine(callerLine) { }
};

#define DEBUG_INFO DebugInfo(__FILE__, __func__, __LINE__)

void ImEdLogImpl(const std::string& message, DebugMessageType messageType, DebugInfo debugInfo);
#define ImEdLog(__message, __messageType) ImEdLogImpl(__message, __messageType, DEBUG_INFO)

#define ImEdAssert(__expression__, __error_message__) \
		if (__expression__) { ImEdLog(fmt::format("Assertion failed \"{}\": {}", #__expression__, __error_message__), DebugMessageType::Error); }
#define ImEdAssertFatal(__expression__, __error_message__) \
		if (__expression__) { ImEdLog(fmt::format("Assertion failed \"{}\"", #__expression__, __error_message__), DebugMessageType::FatalError); }

class OptionUnwrapFailureException : public std::exception {
public:
	[[nodiscard]] inline const char* what() const override { return "Attempt to unwrap option with \"None\" value!"; }
};

template<typename T>
class Option {
	static_assert(!std::is_same<T, std::nullopt_t>::value, "Unexpected template type!");
	std::variant<std::nullopt_t, T> m_value;
public:
	Option(T&& value): m_value({ std::move(value) }) { }			// NOLINT: Implicit conversion
	Option(const T& value): m_value({ value }) { }					// NOLINT: Implicit conversion
	Option(const std::nullopt_t&): m_value({ std::nullopt }) { }	// NOLINT: Implicit conversion

	[[nodiscard]] inline constexpr bool is_some() const { return m_value.index() != 0; }
	[[nodiscard]] inline constexpr bool is_none() const { return m_value.index() == 0; }

	inline constexpr T unwrap() const { return is_some() ? std::get<1>(m_value) : throw OptionUnwrapFailureException(); }
	inline constexpr T unwrap_or(T&& _default) const { return is_some() ? std::get<1>(m_value) : std::move(_default); }
	inline constexpr T unwrap_or(const T& _default) const { return is_some() ? std::get<1>(m_value) : _default; }

	inline static constexpr std::nullopt_t None = std::nullopt;
};