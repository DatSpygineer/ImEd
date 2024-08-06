#include "imed_gui_texteditor.hpp"

TextEditor::TextEditor(size_t bufferCapacity) {
	m_buffer.reserve(bufferCapacity);
}

TextEditor::TextEditor(const std::string& initialText, size_t bufferCapacity) : m_buffer(initialText){
	if (initialText.size() < bufferCapacity) {
		m_buffer.reserve(bufferCapacity);
	}
}

void TextEditor::show() {
	ImGui::InputTextMultiline("##text", m_buffer.data(), m_buffer.capacity());
}
