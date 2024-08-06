#include <codecvt>

#include "ImEdNativeDiag.hpp"

#ifdef _WIN32
	#include <windows.h>
	#include <shobjidl.h>
	#ifdef MessageBox
		#undef MessageBox
	#endif
#else

extern "C" {
	#include <gtk/gtk.h>
}
#endif

static std::wstring StrToWStr(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}
static std::string WStrToStr(const std::wstring& str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(str);
}

MessageBoxResult MessageBox::Show(const std::string& title, const std::string& message, MessageBoxType type,
	MessageBoxButtons buttons) {
#ifdef _WIN32
	uint32_t flags = 0;
	switch (type) {
		case MessageBoxType::Info:     flags = 0x40; break;
		case MessageBoxType::Warning:  flags = 0x30; break;
		case MessageBoxType::Question: flags = 0x20; break;
		case MessageBoxType::Error:    flags = 0x10; break;
	}
	switch (buttons) {
		case MessageBoxButtons::Ok:          flags |= 0x00; break;
		case MessageBoxButtons::OkCancel:    flags |= 0x01; break;
		case MessageBoxButtons::YesNo:       flags |= 0x04; break;
		case MessageBoxButtons::YesNoCancel: flags |= 0x03; break;
	}
	switch (MessageBoxA(nullptr, message.c_str(), title.c_str(), flags)) {
		case 1: return MessageBoxResult::Ok;
		case 2: return MessageBoxResult::Cancel;
		case 6: return MessageBoxResult::Yes;
		case 7: return MessageBoxResult::No;
		default: return MessageBoxResult::Invalid;
	}
#else
	gtk_init(nullptr, nullptr);

	GtkMessageType gType;
	switch (type) {
		default: gType = GTK_MESSAGE_INFO; break;
		case MessageBoxType::Warning: gType = GTK_MESSAGE_WARNING; break;
		case MessageBoxType::Error: gType = GTK_MESSAGE_ERROR; break;
		case MessageBoxType::Question: gType = GTK_MESSAGE_QUESTION; break;
	}
	GtkButtonsType gButtons;
	switch (buttons) {
		default: gButtons = GTK_BUTTONS_OK; break;
		case MessageBoxButtons::OkCancel: gButtons = GTK_BUTTONS_OK_CANCEL; break;
		case MessageBoxButtons::YesNo:  gButtons = GTK_BUTTONS_YES_NO; break;
		case MessageBoxButtons::YesNoCancel: gButtons = GTK_BUTTONS_NONE; break;
	}

	GtkWidget* msgBox = gtk_message_dialog_new(nullptr, GTK_DIALOG_DESTROY_WITH_PARENT, gType, gButtons, message.c_str());
	if (buttons == MessageBoxButtons::YesNoCancel) {
		gtk_dialog_add_button(GTK_DIALOG(msgBox), "Yes", GTK_RESPONSE_YES);
		gtk_dialog_add_button(GTK_DIALOG(msgBox), "No", GTK_RESPONSE_NO);
		gtk_dialog_add_button(GTK_DIALOG(msgBox), "Cancel", GTK_RESPONSE_CANCEL);
	}

	gint result = gtk_dialog_run(GTK_DIALOG(msgBox));
	gtk_widget_destroy(msgBox);

	switch (result) {
		case GTK_RESPONSE_OK: return MessageBoxResult::Ok;
		case GTK_RESPONSE_CANCEL: return MessageBoxResult::Cancel;
		case GTK_RESPONSE_YES: return MessageBoxResult::Yes;
		case GTK_RESPONSE_NO: return MessageBoxResult::No;
		default: return MessageBoxResult::Invalid;
	}
#endif
}

std::vector<std::filesystem::path> FileDialog::Show(const std::string& title, FileDialogType type, bool allowMultiple,
	const std::vector<FileFilter>& filters) {
	std::vector<std::filesystem::path> paths = { };
#ifdef _WIN32
	std::vector<COMDLG_FILTERSPEC> filtersWin;
	if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
		return paths;
	}

	IFileDialog* diag;

	const _GUID typeFlag = type == FileDialogType::SaveFile ? CLSID_FileSaveDialog : CLSID_FileOpenDialog;
	const IID interfaceType = type == FileDialogType::SaveFile ?  IID_IFileSaveDialog : IID_IFileOpenDialog;

	if (!SUCCEEDED(CoCreateInstance(typeFlag, nullptr, CLSCTX_ALL, interfaceType, reinterpret_cast<void**>(&diag)))) {
		return paths;
	}

	DWORD flags;
	diag->GetOptions(&flags);
	if (allowMultiple) {
		flags |= FOS_ALLOWMULTISELECT;
	}

	switch (type) {
		case FileDialogType::OpenFile: {
			flags |= FOS_FILEMUSTEXIST;
		} break;
		case FileDialogType::Directory: {
			flags |= FOS_PICKFOLDERS;
		} break;
	}
	diag->SetOptions(flags);

	if (!filters.empty()) {
		filtersWin.reserve(filters.size());

		for (const auto& filter : filters) {
			filtersWin.push_back({
				filter.name.c_str(),
				filter.pattern.c_str()
			});
		}

		diag->SetFileTypes(filtersWin.size(), filtersWin.data());
		diag->SetFileTypeIndex(1);
		diag->SetDefaultExtension(filtersWin.front().pszSpec);
	}

	if (!SUCCEEDED(diag->Show(nullptr))) {
		diag->Release();
		return paths;
	}

	if (type == FileDialogType::SaveFile) {
		IShellItem* item;
		if (!SUCCEEDED(reinterpret_cast<IFileSaveDialog*>(diag)->GetResult(&item))) {
			diag->Release();
			return paths;
		}
		PWSTR path;
		if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
			paths.emplace_back(path);
			CoTaskMemFree(path);
		}
		item->Release();
	} else {
		IShellItemArray* results;
		if (!SUCCEEDED(reinterpret_cast<IFileOpenDialog*>(diag)->GetResults(&results))) {
			diag->Release();
			return paths;
		}
		DWORD resultSize;
		if (!SUCCEEDED(results->GetCount(&resultSize))) {
			results->Release();
			diag->Release();
			return paths;
		}

		for (DWORD i = 0; i < resultSize; i++) {
			IShellItem* item;
			if (SUCCEEDED(results->GetItemAt(i, &item))) {
				PWSTR path;
				if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
					paths.emplace_back(path);
					CoTaskMemFree(path);
				}
				item->Release();
			}
		}
		results->Release();
	}

	diag->Release();

	CoUninitialize();
#else
	gtk_init(nullptr, nullptr);
	GtkFileChooserAction action;
	switch (type) {
		case FileDialogType::OpenFile: action = GTK_FILE_CHOOSER_ACTION_OPEN; break;
		case FileDialogType::SaveFile: action = GTK_FILE_CHOOSER_ACTION_SAVE; break;
		case FileDialogType::Directory: action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER; break;
		default: return paths;
	}

	GtkWidget* diag = gtk_file_chooser_dialog_new(title.c_str(), nullptr, action,
			"Cancel",
			GTK_RESPONSE_CANCEL,
			"Open",
			GTK_RESPONSE_ACCEPT,
			NULL
	);

	GtkFileChooser* chooser = GTK_FILE_CHOOSER(diag);
	gtk_file_chooser_set_select_multiple(chooser, allowMultiple);

	if (!filters.empty()) {
		for (auto ff : filters) {
			GtkFileFilter* filter = gtk_file_filter_new();
			gtk_file_filter_set_name(filter, (ff.name + " (" + ff.pattern + ")").c_str());

			std::istringstream pattern;
			pattern.str(ff.pattern);
			std::string current_pattern;

			while (std::getline(pattern, current_pattern, ';')) {
				gtk_file_filter_add_pattern(filter, current_pattern.c_str());
			}
			gtk_file_chooser_add_filter(chooser, filter);
		}
	}

	gint result = gtk_dialog_run(GTK_DIALOG(diag));
	if (result == GTK_RESPONSE_ACCEPT) {
		if (allowMultiple) {
			GSList* names = gtk_file_chooser_get_filenames(chooser);
			GSList* iter = names;

			while (iter != nullptr) {
				if (iter->data != nullptr) {
					paths.emplace_back(static_cast<const char*>(iter->data));
				}
				iter = iter->next;
			}
		} else {
			char* filename = gtk_file_chooser_get_filename (chooser);
			if (filename != nullptr) {
				paths.emplace_back(filename);
				g_free(filename);
			}
		}
	}
	gtk_widget_destroy(diag);
#endif
	return paths;
}

std::vector<std::filesystem::path> FileDialog::Show(const std::string& title, FileDialogType type, bool allowMultiple,
										const std::initializer_list<FileFilter>& filters) {
	std::vector<FileFilter> filtersVec = { };
	if (filters.size() > 0) {
		filtersVec.reserve(filters.size());
		for (const auto& filter : filters) {
			filtersVec.push_back(filter);
		}
	}
	return FileDialog::Show(title, type, allowMultiple, filtersVec);
}

#ifdef _WIN32
FileFilter::FileFilter(const std::string& name, const std::string& pattern):
	FileFilter(StrToWStr(name), StrToWStr(pattern)) {
}
FileFilter::FileFilter(std::string&& name, std::string&& pattern):
	FileFilter(StrToWStr(name), StrToWStr(pattern)) {
}
FileFilter::FileFilter(const std::wstring& name, const std::wstring& pattern): name(name), pattern(pattern) { }
FileFilter::FileFilter(std::wstring&& name, std::wstring&& pattern): name(std::move(name)), pattern(std::move(pattern)) { }
#else
FileFilter::FileFilter(const std::string& name, const std::string& pattern): name(name), pattern(pattern) { }
FileFilter::FileFilter(std::string&& name, std::string&& pattern): name(std::move(name)), pattern(std::move(pattern)) { }
#endif