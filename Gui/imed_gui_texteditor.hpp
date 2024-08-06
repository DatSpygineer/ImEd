#pragma once

#include "imed_gui_layout.hpp"

class TextEditor : public IWidget {
	std::string m_buffer;
public:
	explicit TextEditor(size_t bufferCapacity);
	TextEditor(const std::string& initialText, size_t bufferCapacity);

	inline std::string& buffer() { return m_buffer; }
	inline const std::string& buffer() const { return m_buffer; }

	void show() override;
};