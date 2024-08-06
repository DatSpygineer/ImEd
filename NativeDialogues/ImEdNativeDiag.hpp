#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>

#ifdef _WIN32
	#ifdef MessageBox
		#undef MessageBox
	#endif
#endif

enum class MessageBoxButtons {
	Ok,
	OkCancel,
	YesNo,
	YesNoCancel
};
enum class MessageBoxResult {
	Invalid, Ok, Cancel, Yes, No
};
enum class MessageBoxType {
	Info, Warning, Error, Question
};

namespace MessageBox {
	MessageBoxResult Show(const std::string& title, const std::string& message, MessageBoxType type, MessageBoxButtons buttons = MessageBoxButtons::Ok);
	inline MessageBoxResult Info(const std::string& title, const std::string& message, MessageBoxButtons buttons = MessageBoxButtons::Ok) {
		return Show(title, message, MessageBoxType::Info, buttons);
	}
	inline MessageBoxResult Warning(const std::string& title, const std::string& message, MessageBoxButtons buttons = MessageBoxButtons::Ok) {
		return Show(title, message, MessageBoxType::Warning, buttons);
	}
	inline MessageBoxResult Error(const std::string& title, const std::string& message, MessageBoxButtons buttons = MessageBoxButtons::Ok) {
		return Show(title, message, MessageBoxType::Error, buttons);
	}
	inline MessageBoxResult Question(const std::string& title, const std::string& message, MessageBoxButtons buttons = MessageBoxButtons::YesNo) {
		return Show(title, message, MessageBoxType::Question, buttons);
	}
}

enum class FileDialogType {
	OpenFile,
	SaveFile,
	Directory
};
using FileDialogueType = FileDialogType;

struct FileFilter {
#ifdef _WIN32
	std::wstring name, pattern;
#else
	std::string name, pattern;
#endif

	FileFilter(const std::string& name, const std::string& pattern);
	FileFilter(std::string&& name, std::string&& pattern);
#ifdef _WIN32
	FileFilter(const std::wstring& name, const std::wstring& pattern);
	FileFilter(std::wstring&& name, std::wstring&& pattern);
#endif
};

namespace FileDialog {
	std::vector<std::filesystem::path> Show(const std::string& title, FileDialogType type, bool allowMultiple,
		const std::initializer_list<FileFilter>& filters);
	std::vector<std::filesystem::path> Show(const std::string& title, FileDialogType type, bool allowMultiple,
		const std::vector<FileFilter>& filters);
}

namespace FileDialogue = FileDialog;
