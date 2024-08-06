#include "imed_gui_types.hpp"

#include <vector>
#include <sstream>

Option<Color> Color::FromString(const std::string& text) {
	if (text.starts_with('#')) {
		return Color { std::stoul(text.substr(1), nullptr, 16) };
	} else if (text.starts_with("rgba")) {
		auto literal = text.substr(4);
		if (literal.starts_with('(')) {
			literal = literal.substr(1);
		}
		if (literal.ends_with(')')) {
			literal = literal.substr(0, literal.size() - 1);
		}

		std::vector<std::string> tokens;
		std::istringstream iss;
		iss.str(literal);
		std::string token;
		while (std::getline(iss, token, ',')) {
			tokens.push_back(token);
		}

		ImEdAssert(tokens.size() == 4, "Expected 4 elements in color literal " + text);

		try {
			return {
				Color{
					std::stof(tokens.at(0)),
					std::stof(tokens.at(1)),
					std::stof(tokens.at(2)),
					std::stof(tokens.at(3))
				}
			};
		} catch (const std::exception& _) {
			return Option<Color>::None;
		}
	} else if (text.starts_with("rgb")) {
		auto literal = text.substr(3);
		if (literal.starts_with('(')) {
			literal = literal.substr(1);
		}
		if (literal.ends_with(')')) {
			literal = literal.substr(0, literal.size() - 1);
		}

		std::vector<std::string> tokens;
		std::istringstream iss;
		iss.str(literal);
		std::string token;
		while (std::getline(iss, token, ',')) {
			tokens.push_back(token);
		}

		ImEdAssert(tokens.size() == 3, "Expected 3 elements in color literal " + text);

		try {
			return {
				Color{
					std::stof(tokens.at(0)),
					std::stof(tokens.at(1)),
					std::stof(tokens.at(2))
				}
			};
		} catch (const std::exception& _) {
			return Option<Color>::None;
		}
	} else if (text.starts_with("hsla")) {
		auto literal = text.substr(4);
		if (literal.starts_with('(')) {
			literal = literal.substr(1);
		}
		if (literal.ends_with(')')) {
			literal = literal.substr(0, literal.size() - 1);
		}

		std::vector<std::string> tokens;
		std::istringstream iss;
		iss.str(literal);
		std::string token;
		while (std::getline(iss, token, ',')) {
			tokens.push_back(token);
		}

		ImEdAssert(tokens.size() == 4, "Expected 4 elements in color literal " + text);

		if (tokens.at(1).ends_with("%")) {
			auto str = tokens.at(1);
			tokens.at(1) = str.substr(0, str.size() - 1);
		}
		if (tokens.at(2).ends_with("%")) {
			auto str = tokens.at(2);
			tokens.at(2) = str.substr(0, str.size() - 1);
		}

		try {
			return {
				Color::FromHsv(
					std::stof(tokens.at(0)),
					std::stof(tokens.at(1)) / 100.0f,
					std::stof(tokens.at(2)) / 100.0f,
					std::stof(tokens.at(3))
				)
			};
		} catch (const std::exception& _) {
			return Option<Color>::None;
		}
	} else if (text.starts_with("hsl")) {
		auto literal = text.substr(4);
		if (literal.starts_with('(')) {
			literal = literal.substr(1);
		}
		if (literal.ends_with(')')) {
			literal = literal.substr(0, literal.size() - 1);
		}

		std::vector<std::string> tokens;
		std::istringstream iss;
		iss.str(literal);
		std::string token;
		while (std::getline(iss, token, ',')) {
			tokens.push_back(token);
		}

		ImEdAssert(tokens.size() == 4, "Expected 4 elements in color literal " + text);

		if (tokens.at(1).ends_with("%")) {
			auto str = tokens.at(1);
			tokens.at(1) = str.substr(0, str.size() - 1);
		}
		if (tokens.at(2).ends_with("%")) {
			auto str = tokens.at(2);
			tokens.at(2) = str.substr(0, str.size() - 1);
		}

		try {
			return {
				Color::FromHsv(
					std::stof(tokens.at(0)),
					std::stof(tokens.at(1)) / 100.0f,
					std::stof(tokens.at(2)) / 100.0f
				)
			};
		} catch (const std::exception& _) {
			return Option<Color>::None;
		}
	} else {
		return Option<Color>::None;
	}
}

Color Color::FromHsv(float h, float s, float v, float a) {
	auto c = v * s;
	auto x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
	auto m = v - c;

	if (h >= 0 && h < 60) {
		return { c, x, 0, a };
	} else if (h >= 60 && h < 120) {
		return { x, c, 0, a };
	} else if (h >= 120 && h < 180) {
		return { 0, c, x, a };
	} else if (h >= 180 && h < 240) {
		return { 0, x, c, a };
	} else if (h >= 240 && h < 300) {
		return { x, 0, c, a };
	} else {
		return {c, 0, x, a };
	}
}
void Color::to_hsv(float& h, float& s, float& v) {
	auto max = std::max({ r, g, b });
	auto min = std::max({ r, g, b });
	auto delta = max - min;

	v = max;
	if (max == 0) {
		s = 0;
	} else {
		s = delta / max;
	}

	if (delta == 0) {
		h = 0;
	} else {
		if (max == r) {
			h = 60.0f * std::fmod(((g - b) / delta), 6.0f);
		} else if (max == g) {
			h = 60.0f * ((b - r) / delta) + 2;
		} else if (max == b) {
			h = 60.0f * ((r - g) / delta) + 4;
		}

		if (h < 0.0f) {
			h += 360.0f;
		}
	}
}

ImGuiStyle Style::toImGuiStyle() const {
	ImGuiStyle imGuiStyle;
	imGuiStyle.Alpha                       = Alpha;
	imGuiStyle.DisabledAlpha               = DisabledAlpha;
	imGuiStyle.WindowPadding               = WindowPadding;
	imGuiStyle.WindowRounding              = WindowRounding;
	imGuiStyle.WindowBorderSize            = WindowBorderSize;
	imGuiStyle.WindowMinSize               = WindowMinSize;
	imGuiStyle.WindowTitleAlign            = WindowTitleAlign;
	imGuiStyle.WindowMenuButtonPosition    = WindowMenuButtonPosition;
	imGuiStyle.ChildRounding               = ChildRounding;
	imGuiStyle.ChildBorderSize             = ChildBorderSize;
	imGuiStyle.PopupRounding               = PopupRounding;
	imGuiStyle.PopupBorderSize             = PopupBorderSize;
	imGuiStyle.FramePadding                = FramePadding;
	imGuiStyle.FrameRounding               = FrameRounding;
	imGuiStyle.FrameBorderSize             = FrameBorderSize;
	imGuiStyle.ItemSpacing                 = ItemSpacing;
	imGuiStyle.ItemInnerSpacing            = ItemInnerSpacing;
	imGuiStyle.CellPadding                 = CellPadding;
	imGuiStyle.TouchExtraPadding           = TouchExtraPadding;
	imGuiStyle.IndentSpacing               = IndentSpacing;
	imGuiStyle.ColumnsMinSpacing           = ColumnsMinSpacing;
	imGuiStyle.ScrollbarSize               = ScrollbarSize;
	imGuiStyle.ScrollbarRounding           = ScrollbarRounding;
	imGuiStyle.GrabMinSize                 = GrabMinSize;
	imGuiStyle.GrabRounding                = GrabRounding;
	imGuiStyle.LogSliderDeadzone           = LogSliderDeadzone;
	imGuiStyle.TabRounding                 = TabRounding;
	imGuiStyle.TabBorderSize               = TabBorderSize;
	imGuiStyle.TabMinWidthForCloseButton   = TabMinWidthForCloseButton;
	imGuiStyle.TabBarBorderSize            = TabBarBorderSize;
	imGuiStyle.TableAngledHeadersAngle     = TableAngledHeadersAngle;
	imGuiStyle.TableAngledHeadersTextAlign = TableAngledHeadersTextAlign;
	imGuiStyle.ColorButtonPosition         = ColorButtonPosition;
	imGuiStyle.ButtonTextAlign             = ButtonTextAlign;
	imGuiStyle.SelectableTextAlign         = SelectableTextAlign;
	imGuiStyle.SeparatorTextBorderSize     = SeparatorTextBorderSize;
	imGuiStyle.SeparatorTextAlign          = SeparatorTextAlign;
	imGuiStyle.SeparatorTextPadding        = SeparatorTextPadding;
	imGuiStyle.DisplayWindowPadding        = DisplayWindowPadding;
	imGuiStyle.DisplaySafeAreaPadding      = DisplaySafeAreaPadding;
	imGuiStyle.MouseCursorScale            = MouseCursorScale;
	imGuiStyle.AntiAliasedLines            = AntiAliasedLines;
	imGuiStyle.AntiAliasedLinesUseTex      = AntiAliasedLinesUseTex;
	imGuiStyle.AntiAliasedFill             = AntiAliasedFill;
	imGuiStyle.CurveTessellationTol        = CurveTessellationTol;
	imGuiStyle.CircleTessellationMaxError  = CircleTessellationMaxError;
	imGuiStyle.HoverStationaryDelay        = HoverStationaryDelay;
	imGuiStyle.HoverDelayShort             = HoverDelayShort;
	imGuiStyle.HoverDelayNormal            = HoverDelayNormal;
	imGuiStyle.HoverFlagsForTooltipMouse   = HoverFlagsForTooltipMouse;
	imGuiStyle.HoverFlagsForTooltipNav     = HoverFlagsForTooltipNav;

	imGuiStyle.Colors[ImGuiCol_Text]                      = Colors.TextColor;
	imGuiStyle.Colors[ImGuiCol_TextDisabled]              = Colors.TextColorDisabled;
	imGuiStyle.Colors[ImGuiCol_WindowBg]                  = Colors.WindowBg;
	imGuiStyle.Colors[ImGuiCol_ChildBg]                   = Colors.ChildBg;
	imGuiStyle.Colors[ImGuiCol_PopupBg]                   = Colors.PopupBg;
	imGuiStyle.Colors[ImGuiCol_Border]                    = Colors.BorderColor;
	imGuiStyle.Colors[ImGuiCol_BorderShadow]              = Colors.BorderShadowColor;
	imGuiStyle.Colors[ImGuiCol_FrameBg]                   = Colors.FrameBg;
	imGuiStyle.Colors[ImGuiCol_FrameBgHovered]            = Colors.FrameBgHovered;
	imGuiStyle.Colors[ImGuiCol_FrameBgActive]             = Colors.FrameBgActive;
	imGuiStyle.Colors[ImGuiCol_TitleBg]                   = Colors.TitleBg;
	imGuiStyle.Colors[ImGuiCol_TitleBgActive]             = Colors.TitleBgActive;
	imGuiStyle.Colors[ImGuiCol_TitleBgCollapsed]          = Colors.TitleBgCollapsed;
	imGuiStyle.Colors[ImGuiCol_MenuBarBg]                 = Colors.MenubarBg;
	imGuiStyle.Colors[ImGuiCol_ScrollbarBg]               = Colors.ScrollbarBg;
	imGuiStyle.Colors[ImGuiCol_ScrollbarGrab]             = Colors.ScrollbarGrabColor;
	imGuiStyle.Colors[ImGuiCol_ScrollbarGrabHovered]      = Colors.ScrollbarGrabColorHovered;
	imGuiStyle.Colors[ImGuiCol_ScrollbarGrabActive]       = Colors.ScrollbarGrabColorActive;
	imGuiStyle.Colors[ImGuiCol_CheckMark]                 = Colors.CheckMarkColor;
	imGuiStyle.Colors[ImGuiCol_SliderGrab]                = Colors.SliderGrabColor;
	imGuiStyle.Colors[ImGuiCol_SliderGrabActive]          = Colors.SliderGrabColorActive;
	imGuiStyle.Colors[ImGuiCol_Button]                    = Colors.ButtonColor;
	imGuiStyle.Colors[ImGuiCol_ButtonHovered]             = Colors.ButtonColorHovered;
	imGuiStyle.Colors[ImGuiCol_ButtonActive]              = Colors.ButtonColorActive;
	imGuiStyle.Colors[ImGuiCol_Header]                    = Colors.HeaderColor;
	imGuiStyle.Colors[ImGuiCol_HeaderHovered]             = Colors.HeaderColorHovered;
	imGuiStyle.Colors[ImGuiCol_HeaderActive]              = Colors.HeaderColorActive;
	imGuiStyle.Colors[ImGuiCol_Separator]                 = Colors.SeparatorColor;
	imGuiStyle.Colors[ImGuiCol_SeparatorHovered]          = Colors.SeparatorColorHovered;
	imGuiStyle.Colors[ImGuiCol_SeparatorActive]           = Colors.SeparatorColorActive;
	imGuiStyle.Colors[ImGuiCol_ResizeGrip]                = Colors.ResizeGripColor;
	imGuiStyle.Colors[ImGuiCol_ResizeGripHovered]         = Colors.ResizeGripColorHovered;
	imGuiStyle.Colors[ImGuiCol_ResizeGripActive]          = Colors.ResizeGripColorActive;
	imGuiStyle.Colors[ImGuiCol_TabHovered]                = Colors.TabColorHovered;
	imGuiStyle.Colors[ImGuiCol_Tab]                       = Colors.TabColor;
	imGuiStyle.Colors[ImGuiCol_TabSelected]               = Colors.TabColorSelected;
	imGuiStyle.Colors[ImGuiCol_TabSelectedOverline]       = Colors.TabColorSelectedOverline;
	imGuiStyle.Colors[ImGuiCol_TabDimmed]                 = Colors.TabColorDimmed;
	imGuiStyle.Colors[ImGuiCol_TabDimmedSelected]         = Colors.TabColorDimmedSelected;
	imGuiStyle.Colors[ImGuiCol_TabDimmedSelectedOverline] = Colors.TabColorDimmedSelectedOverline;
	imGuiStyle.Colors[ImGuiCol_PlotLines]                 = Colors.PlotLinesColor;
	imGuiStyle.Colors[ImGuiCol_PlotLinesHovered]          = Colors.PlotLinesColorHovered;
	imGuiStyle.Colors[ImGuiCol_PlotHistogram]             = Colors.PlotHistogramColor;
	imGuiStyle.Colors[ImGuiCol_PlotHistogramHovered]      = Colors.PlotHistogramColorHovered;
	imGuiStyle.Colors[ImGuiCol_TableHeaderBg]             = Colors.TableHeaderBg;
	imGuiStyle.Colors[ImGuiCol_TableBorderStrong]         = Colors.TableBorderColorStrong;
	imGuiStyle.Colors[ImGuiCol_TableBorderLight]          = Colors.TableBorderColorLight;
	imGuiStyle.Colors[ImGuiCol_TableRowBg]                = Colors.TableRowBg;
	imGuiStyle.Colors[ImGuiCol_TableRowBgAlt]             = Colors.TableRowBgAlt;
	imGuiStyle.Colors[ImGuiCol_TextSelectedBg]            = Colors.TextSelectedBg;
	imGuiStyle.Colors[ImGuiCol_DragDropTarget]            = Colors.DragDropTargetColor;
	imGuiStyle.Colors[ImGuiCol_NavHighlight]              = Colors.NavHighlightColor;
	imGuiStyle.Colors[ImGuiCol_NavWindowingHighlight]     = Colors.NavWindowingHighlightColor;
	imGuiStyle.Colors[ImGuiCol_NavWindowingDimBg]         = Colors.NavWindowingDimBg;
	imGuiStyle.Colors[ImGuiCol_ModalWindowDimBg]          = Colors.ModalWindowDimBg;

	return imGuiStyle;
}

Style Style::FromImGuiStyle(const ImGuiStyle& imGuiStyle) {
	return Style {
		imGuiStyle.Alpha,
		imGuiStyle.DisabledAlpha,
		imGuiStyle.WindowPadding,
		imGuiStyle.WindowRounding,
		imGuiStyle.WindowBorderSize,
		imGuiStyle.WindowMinSize,
		imGuiStyle.WindowTitleAlign,
		imGuiStyle.WindowMenuButtonPosition,
		imGuiStyle.ChildRounding,
		imGuiStyle.ChildBorderSize,
		imGuiStyle.PopupRounding,
		imGuiStyle.PopupBorderSize,
		imGuiStyle.FramePadding,
		imGuiStyle.FrameRounding,
		imGuiStyle.FrameBorderSize,
		imGuiStyle.ItemSpacing,
		imGuiStyle.ItemInnerSpacing,
		imGuiStyle.CellPadding,
		imGuiStyle.TouchExtraPadding,
		imGuiStyle.IndentSpacing,
		imGuiStyle.ColumnsMinSpacing,
		imGuiStyle.ScrollbarSize,
		imGuiStyle.ScrollbarRounding,
		imGuiStyle.GrabMinSize,
		imGuiStyle.GrabRounding,
		imGuiStyle.LogSliderDeadzone,
		imGuiStyle.TabRounding,
		imGuiStyle.TabBorderSize,
		imGuiStyle.TabMinWidthForCloseButton,
		imGuiStyle.TabBarBorderSize,
		imGuiStyle.TableAngledHeadersAngle,
		imGuiStyle.TableAngledHeadersTextAlign,
		imGuiStyle.ColorButtonPosition,
		imGuiStyle.ButtonTextAlign,
		imGuiStyle.SelectableTextAlign,
		imGuiStyle.SeparatorTextBorderSize,
		imGuiStyle.SeparatorTextAlign,
		imGuiStyle.SeparatorTextPadding,
		imGuiStyle.DisplayWindowPadding,
		imGuiStyle.DisplaySafeAreaPadding,
		imGuiStyle.MouseCursorScale,
		imGuiStyle.AntiAliasedLines,
		imGuiStyle.AntiAliasedLinesUseTex,
		imGuiStyle.AntiAliasedFill,
		imGuiStyle.CurveTessellationTol,
		imGuiStyle.CircleTessellationMaxError,
		imGuiStyle.HoverStationaryDelay,
		imGuiStyle.HoverDelayShort,
		imGuiStyle.HoverDelayNormal,
		imGuiStyle.HoverFlagsForTooltipMouse,
		imGuiStyle.HoverFlagsForTooltipNav,

		StyleColors {
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Text]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TextDisabled]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_WindowBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ChildBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_PopupBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Border]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_BorderShadow]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_FrameBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_FrameBgHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_FrameBgActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TitleBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TitleBgActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TitleBgCollapsed]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_MenuBarBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ScrollbarBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ScrollbarGrab]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ScrollbarGrabHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ScrollbarGrabActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_CheckMark]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_SliderGrab]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_SliderGrabActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Button]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ButtonHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ButtonActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Header]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_HeaderHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_HeaderActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Separator]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_SeparatorHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_SeparatorActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ResizeGrip]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ResizeGripHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ResizeGripActive]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_Tab]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabSelected]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabSelectedOverline]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabDimmed]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabDimmedSelected]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TabDimmedSelectedOverline]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_PlotLines]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_PlotLinesHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_PlotHistogram]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_PlotHistogramHovered]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TableHeaderBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TableBorderStrong]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TableBorderLight]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TableRowBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TableRowBgAlt]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_TextSelectedBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_DragDropTarget]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_NavHighlight]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_NavWindowingHighlight]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_NavWindowingDimBg]),
			static_cast<Color>(imGuiStyle.Colors[ImGuiCol_ModalWindowDimBg])
		}
	};
}

Style Style::GetCurrentStyle() {
	return FromImGuiStyle(ImGui::GetStyle());
}

void Style::setCurrent() const {
	ImGui::GetStyle() = toImGuiStyle();
}

Image::Image(const std::filesystem::path& path): m_bCopy(false) {
	int w, h, c;
	auto data = stbi_load(path.string().c_str(), &w, &h, &c, STBI_default);

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	GLenum format;
	switch (c) {
		case 1: format = GL_R;
		case 2: format = GL_RG;
		case 3: format = GL_RGB;
		default: format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GLint(format), w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}
Image::~Image() {
	if (m_id != 0 && !m_bCopy) {
		glDeleteTextures(1, &m_id);
	}
}