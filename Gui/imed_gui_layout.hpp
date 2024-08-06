#pragma once

#include "imed_gui_common.hpp"
#include "imed_gui_types.hpp"

#include <filesystem>

struct IWidget {
	virtual void show() = 0;
};

template<typename T>
concept WidgetType = std::is_assignable_v<IWidget, T>;

class Layout : public IWidget {
protected:
	std::vector<std::shared_ptr<IWidget>> m_widgets;
public:

	template<WidgetType T>
	void addWidget(T&& widget);
	template<WidgetType T>
	void addWidget(const T& widget);

	void show() override;
};

class HLayout : public Layout {
public:
	void show() override;
};

class Label : public IWidget {
public:
	std::string label;

	explicit Label(std::string&& label) noexcept: label(std::move(label)) { }
	explicit Label(const std::string& label): label(label) { }

	void show() override;
};

class Separator : public IWidget {
public:
	std::string label;

	explicit Separator(std::string&& label) noexcept: label(std::move(label)) { }
	explicit Separator(const std::string& label = ""): label(label) { }

	void show() override;
};

class TextInput : public IWidget {
	std::string m_buffer;
public:
	std::string label;

	explicit TextInput(const std::string& label, size_t bufferSize);

	std::string& buffer() { return m_buffer; }
	[[nodiscard]] const std::string& buffer() const { return m_buffer; }

	ImGuiInputTextCallback callback = nullptr;
	ImGuiInputTextFlags flags = 0;

	void show() override;
};
class IntInput : public IWidget {
public:
	std::string label;

	explicit IntInput(std::string&& label, int initialValue): label(std::move(label)), value(initialValue) { }
	explicit IntInput(const std::string& label, int initialValue): label(label), value(initialValue) { }

	ImGuiInputTextFlags flags = 0;

	int value;
	int step     = 1;
	int stepFast = 100;

	void show() override;
};
class FloatInput : public IWidget {
public:
	std::string label;

	explicit FloatInput(std::string&& label, float initialValue): label(std::move(label)), value(initialValue) { }
	explicit FloatInput(const std::string& label, float initialValue): label(label), value(initialValue) { }

	ImGuiInputTextFlags flags = 0;

	float value;
	float step     = 0.0f;
	float stepFast = 0.0f;

	void show() override;
};

class IntSlider : public IWidget {
public:
	std::string label;

	explicit IntSlider(std::string&& label, int initialValue): label(std::move(label)), value(initialValue) { }
	explicit IntSlider(const std::string& label, int initialValue): label(label), value(initialValue) { }

	ImGuiSliderFlags flags = 0;

	int value;
	int min = 0;
	int max = 100;

	void show() override;
};
class FloatSlider : public IWidget {
public:
	std::string label;

	explicit FloatSlider(std::string&& label, float initialValue): label(std::move(label)), value(initialValue) { }
	explicit FloatSlider(const std::string& label, float initialValue): label(label), value(initialValue) { }

	ImGuiSliderFlags flags = 0;

	float value;
	float min = 0;
	float max = 100;

	void show() override;
};

class Button : public IWidget {
public:
	Vec2 size = { 0, 0 };
	std::string label;

	Button(std::string&& label, const std::function<void()>& clicked):
		label(std::move(label)), OnClicked(clicked) { }
	Button(const std::string& label, const std::function<void()>& clicked):
		label(label), OnClicked(clicked) { }

	void show() override;

	std::function<void()> OnClicked;
};

class ImageButton : public IWidget {
public:
	Vec2 size = { 0, 0 };
	Image image;

	ImageButton(const Image& image, const Vec2& imageSize, const std::function<void()>& clicked):
		image(image), size(imageSize), OnClicked(clicked) { }

	void show() override;

	std::function<void()> OnClicked;
};

class BulletPoints : public IWidget {
	std::vector<std::string> m_items;
public:
	BulletPoints(const std::initializer_list<std::string>& items);

	void show() override;
};

class MenuItem : public IWidget {
public:
	std::string label, shortcut;
	std::function<void()> onClicked;

	MenuItem(std::string&& label, const std::function<void()>& onClicked): label(std::move(label)), onClicked(onClicked) { }
	MenuItem(const std::string& label, const std::function<void()>& onClicked): label(label), onClicked(onClicked) { }
	MenuItem(std::string&& label, const std::string& shortcut, const std::function<void()>& onClicked):
		label(std::move(label)), shortcut(shortcut), onClicked(onClicked) { }
	MenuItem(const std::string& label, const std::string& shortcut, const std::function<void()>& onClicked):
		label(label), shortcut(shortcut), onClicked(onClicked) { }
	MenuItem(std::string&& label, std::string&& shortcut, const std::function<void()>& onClicked):
		label(std::move(label)), shortcut(std::move(shortcut)), onClicked(onClicked) { }
	MenuItem(const std::string& label, std::string&& shortcut, const std::function<void()>& onClicked):
		label(label), shortcut(std::move(shortcut)), onClicked(onClicked) { }

	void show() override;

	bool enabled = true;
};

class Menu : public IWidget {
	std::vector<std::variant<Separator, MenuItem>> m_menuItems;
public:
	std::string label;

	explicit Menu(std::string&& label): label(std::move(label)) { }
	explicit Menu(const std::string& label): label(label) { }

	void addItem(MenuItem&& item);
	void addItem(const MenuItem& item);
	void addSeparator(Separator&& separator);
	void addSeparator(const Separator& separator);

	void show() override;

	bool enabled = true;
};

class MenuBar : public IWidget {
	bool m_mainbar;
	std::vector<Menu> m_menus;
public:
	explicit MenuBar(bool mainbar = false): m_mainbar(mainbar) { }

	void addMenu(Menu&& menu);
	void addMenu(const Menu& menu);

	void show() override;
};

class TabItem : public IWidget {
public:
	std::string label;

	explicit TabItem(std::string&& label, ImGuiTabItemFlags flags = 0): label(std::move(label)), flags(flags), isOpen(false) { }
	explicit TabItem(const std::string& label, ImGuiTabItemFlags flags = 0): label(label), flags(flags), isOpen(false) { }

	ImGuiTabItemFlags flags;
	bool isOpen;
	std::shared_ptr<IWidget> content;

	void show() override;
};

class TabLayout : public IWidget {
	const char* m_id;
	std::vector<TabItem> m_tabs;
public:
	explicit TabLayout(const char* id, ImGuiTabBarFlags flags = 0): m_id(id), flags(flags) { }

	ImGuiTabBarFlags flags;

	void addTab(TabItem&& item);
	void addTab(const TabItem& item);

	void show() override;
};

struct ColumnWidget {
	int column;
	std::shared_ptr<IWidget> widget;
};

class ColumnLayout : public IWidget {
	int m_columns;
	std::vector<ColumnWidget> m_widgets;
public:
	explicit ColumnLayout(int columnCount): m_columns(columnCount) { }

	template<WidgetType T>
	void addWidget(int column, T&& widget);
	template<WidgetType T>
	void addWidget(int column, const T& widget);

	void show() override;
};

class TreeNode;

class TreeNode : public IWidget {
public:
	std::vector<TreeNode> nodes;
	std::string label;

	TreeNode(const std::string& label, const std::initializer_list<TreeNode>& children);
	TreeNode(const std::string& label, const std::vector<TreeNode>& children);
	TreeNode(std::string&& label, const std::initializer_list<TreeNode>& children);
	TreeNode(std::string&& label, const std::vector<TreeNode>& children);
	TreeNode(const std::string& label, std::vector<TreeNode>&& children);
	TreeNode(std::string&& label, std::vector<TreeNode>&& children);

	std::function<void(const TreeNode&)> OnSelected;

	void show() override;
};

class FreeTreeNode;

class FreeTreeNode : public IWidget {
public:
	std::vector<FreeTreeNode> nodes;
	std::filesystem::path path;

	FreeTreeNode(const std::filesystem::path& path, const std::initializer_list<FreeTreeNode>& children);
	FreeTreeNode(const std::filesystem::path& path, const std::vector<FreeTreeNode>& children);
	FreeTreeNode(std::filesystem::path&& path, const std::initializer_list<FreeTreeNode>& children);
	FreeTreeNode(std::filesystem::path&& path, const std::vector<FreeTreeNode>& children);
	FreeTreeNode(const std::filesystem::path& path, std::vector<FreeTreeNode>&& children);
	FreeTreeNode(std::filesystem::path&& path, std::vector<FreeTreeNode>&& children);

	std::function<void(const FreeTreeNode&)> OnSelected;

	void show() override;

	static FreeTreeNode BuildFromDirPath(const std::filesystem::path& rootPath);
};

void ImEdGui_Init(const std::filesystem::path& basedir);

extern std::function<Image(const std::string&)> FileIconProvider;