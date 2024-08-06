#pragma once

#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "imed_gui_common.hpp"

#include <fmt/format.h>

#include <GL/glew.h>
#include "stb_image.h"

struct Vec2 {
	float x, y;

	constexpr Vec2(): x(0), y(0) { }
	explicit constexpr Vec2(float xy): x(xy), y(xy) { }
	constexpr Vec2(float x, float y): x(x), y(y) { }
	constexpr Vec2(ImVec2 vec): Vec2(vec.x, vec.y) { } // NOLINT: Implicit conversion

	inline constexpr bool operator== (const Vec2& vec) const { return x == vec.x && y == vec.y; }
	inline constexpr bool operator!= (const Vec2& vec) const { return x != vec.x || y != vec.y; }

	inline constexpr Vec2 operator+ (const Vec2& other) const { return { x + other.x, y + other.y }; }
	inline constexpr Vec2 operator- (const Vec2& other) const { return { x - other.x, y - other.y }; }
	inline constexpr Vec2 operator* (const Vec2& other) const { return { x * other.x, y * other.y }; }
	inline constexpr Vec2 operator/ (const Vec2& other) const { return { x / other.x, y / other.y }; }
	inline constexpr Vec2 operator* (float other) const { return { x * other, y * other }; }
	inline constexpr Vec2 operator/ (float other) const { return { x / other, y / other }; }
	inline constexpr Vec2 operator- () const { return { -x, -y }; }

	[[nodiscard]] inline constexpr float length() const { return std::sqrtf(x * x + y * y); }
	[[nodiscard]] inline constexpr float dot(const Vec2& other) const { return x * other.x + y * other.y; }
	[[nodiscard]] inline constexpr float dist(const Vec2& other) const { return (other - *this).length(); }

	inline constexpr operator ImVec2() const { return { x, y }; } // NOLINT: Implicit conversion
};

struct Color {
	float r, g, b, a;

	Color(float r, float g, float b, float a = 1.0f): r(r), g(g), b(b), a(a) { }
	explicit Color(uint32_t rgba):
		r(float((rgba >> 24) & 0xFF) / 255.0f),
		g(float((rgba >> 16) & 0xFF) / 255.0f),
		b(float((rgba >> 8)  & 0xFF) / 255.0f),
		a(float(rgba & 0xFF) / 255.0f) { }
	Color(const ImVec4& vec): r(vec.x), g(vec.y), b(vec.z), a(vec.w) { } // NOLINT: implicit

	static Option<Color> FromString(const std::string& text);
	static Color FromHsv(float h, float s, float v, float a = 1.0f);

	[[nodiscard]] inline constexpr uint32_t to_rgba() const {
		return  (uint32_t(r * 255.0f) << 24) |
				(uint32_t(g * 255.0f) << 16) |
				(uint32_t(b * 255.0f) << 8)  |
				 uint32_t(a * 255.0f);
	}
	void to_hsv(float& h, float& s, float& v);

	inline constexpr bool operator== (const Color& color) const {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}
	inline constexpr bool operator!= (const Color& color) const {
		return r != color.r || g != color.g || b != color.b || a != color.a;
	}

	inline operator ImVec4() const { return { r, g, b, a }; } // NOLINT: implicit
};

template<>
struct fmt::formatter<Vec2> {
	auto format(Vec2 value, fmt::format_context& ctx) {
		return fmt::format_to(ctx.out(), "{}, {}", value.x, value.y);
	}
};

template<>
struct fmt::formatter<Color> {
	auto format(Color value, fmt::format_context& ctx) {
		return fmt::format_to(ctx.out(), "#{:08X}", value.to_rgba());
	}
};

struct StyleColors {
	Color TextColor;
	Color TextColorDisabled;
	Color WindowBg;
	Color ChildBg;
	Color PopupBg;
	Color BorderColor;
	Color BorderShadowColor;
	Color FrameBg;
	Color FrameBgHovered;
	Color FrameBgActive;
	Color TitleBg;
	Color TitleBgActive;
	Color TitleBgCollapsed;
	Color MenubarBg;
	Color ScrollbarBg;
	Color ScrollbarGrabColor;
	Color ScrollbarGrabColorHovered;
	Color ScrollbarGrabColorActive;
	Color CheckMarkColor;
	Color SliderGrabColor;
	Color SliderGrabColorActive;
	Color ButtonColor;
	Color ButtonColorHovered;
	Color ButtonColorActive;
	Color HeaderColor;
	Color HeaderColorHovered;
	Color HeaderColorActive;
	Color SeparatorColor;
	Color SeparatorColorHovered;
	Color SeparatorColorActive;
	Color ResizeGripColor;
	Color ResizeGripColorHovered;
	Color ResizeGripColorActive;
	Color TabColor;
	Color TabColorHovered;
	Color TabColorSelected;
	Color TabColorSelectedOverline;
	Color TabColorDimmed;
	Color TabColorDimmedSelected;
	Color TabColorDimmedSelectedOverline;
	Color PlotLinesColor;
	Color PlotLinesColorHovered;
	Color PlotHistogramColor;
	Color PlotHistogramColorHovered;
	Color TableHeaderBg;
	Color TableBorderColorStrong;
	Color TableBorderColorLight;
	Color TableRowBg;
	Color TableRowBgAlt;
	Color TextSelectedBg;
	Color DragDropTargetColor;
	Color NavHighlightColor;
	Color NavWindowingHighlightColor;
	Color NavWindowingDimBg;
	Color ModalWindowDimBg;
};

struct Style {
	float             Alpha;
	float             DisabledAlpha;
	Vec2              WindowPadding;
	float             WindowRounding;
	float             WindowBorderSize;
	Vec2              WindowMinSize;
	Vec2              WindowTitleAlign;
	ImGuiDir          WindowMenuButtonPosition;
	float             ChildRounding;
	float             ChildBorderSize;
	float             PopupRounding;
	float             PopupBorderSize;
	Vec2              FramePadding;
	float             FrameRounding;
	float             FrameBorderSize;
	Vec2              ItemSpacing;
	Vec2              ItemInnerSpacing;
	Vec2              CellPadding;
	Vec2              TouchExtraPadding;
	float             IndentSpacing;
	float             ColumnsMinSpacing;
	float             ScrollbarSize;
	float             ScrollbarRounding;
	float             GrabMinSize;
	float             GrabRounding;
	float             LogSliderDeadzone;
	float             TabRounding;
	float             TabBorderSize;
	float             TabMinWidthForCloseButton;
	float             TabBarBorderSize;
	float             TableAngledHeadersAngle;
	Vec2              TableAngledHeadersTextAlign;
	ImGuiDir          ColorButtonPosition;
	Vec2              ButtonTextAlign;
	Vec2              SelectableTextAlign;
	float             SeparatorTextBorderSize;
	Vec2              SeparatorTextAlign;
	Vec2              SeparatorTextPadding;
	Vec2              DisplayWindowPadding;
	Vec2              DisplaySafeAreaPadding;
	float             MouseCursorScale;
	bool              AntiAliasedLines;
	bool              AntiAliasedLinesUseTex;
	bool              AntiAliasedFill;
	float             CurveTessellationTol;
	float             CircleTessellationMaxError;
	float             HoverStationaryDelay;
	float             HoverDelayShort;
	float             HoverDelayNormal;
	ImGuiHoveredFlags HoverFlagsForTooltipMouse;
	ImGuiHoveredFlags HoverFlagsForTooltipNav;
	StyleColors       Colors;

	void setCurrent() const;
	[[nodiscard]] ImGuiStyle toImGuiStyle() const;
	static Style FromImGuiStyle(const ImGuiStyle& imGuiStyle);

	static Style GetCurrentStyle();

	inline operator ImGuiStyle() const { return toImGuiStyle(); } // NOLINT: implicit
};

struct CodeStyle {
	Color BackgroundColor;
	Color BackgroundColorSelected;

	Color DefaultTextColor;
	Color DisabledTextColor;

	Color KeywordColor;
	Color KeywordDisabledColor;
	Color TypeNameColor;
	Color TypeNameDisabledColor;
	Color ConstantColor;
	Color ConstantDisabledColor;
	Color VariableColor;
	Color VariableDisabledColor;
	Color MemberColor;
	Color MemberDisabledColor;
	Color FunctionColor;
	Color FunctionDisabledColor;
	Color MemberFunctionColor;
	Color MemberFunctionDisabledColor;
	Color PreprocessorColor;
	Color PreprocessorDisabledColor;

	Color NumericLiteralColor;
	Color NumericLiteralDisabledColor;
	Color CharLiteralColor;
	Color CharLiteralDisabledColor;
	Color StringLiteralColor;
	Color StringLiteralDisabledColor;
	Color StringEscapeCharacterColor;
	Color StringEscapeCharacterDisabledColor;
};

class Image {
	GLuint m_id;
	bool m_bCopy;
public:
	Image(): m_id(0), m_bCopy(false) { }
	explicit Image(const std::filesystem::path& path);
	Image(const Image& image): m_id(image.m_id), m_bCopy(true) { }
	~Image();

	[[nodiscard]] inline constexpr GLuint id() const { return m_id; }
	[[nodiscard]] inline ImTextureID asImTexture() const { return (ImTextureID)m_id; }
};