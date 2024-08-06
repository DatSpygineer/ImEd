#include "imed_gui_layout.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

static Image ImgFolder;
static Image ImgFile;

std::function<Image(const std::string&)> FileIconProvider = nullptr;

template<WidgetType T>
void Layout::addWidget(T&& widget) {
	m_widgets.emplace_back(std::forward(widget));
}
template<WidgetType T>
void Layout::addWidget(const T& widget) {
	m_widgets.emplace_back(widget);
}

void Layout::show() {
	for (auto& widget : m_widgets) {
		widget->show();
	}
}

void HLayout::show() {
	for (size_t i = 0; i < m_widgets.size(); i++) {
		m_widgets.at(i)->show();
		if (i < m_widgets.size() - 1) {
			ImGui::SameLine();
		}
	}
}

void Label::show() {
	ImGui::Text("%s", label.c_str());
}

void Separator::show() {
	if (label.empty()) {
		ImGui::Separator();
	} else {
		ImGui::SeparatorText(label.c_str());
	}
}

TextInput::TextInput(const std::string& label, size_t bufferSize): label(label), m_buffer() {
	m_buffer.reserve(bufferSize);
}
void TextInput::show() {
	ImGui::InputText(label.c_str(), m_buffer.data(), m_buffer.capacity(), flags, callback);
}

void IntInput::show() {
	ImGui::InputInt(label.c_str(), &value, step, stepFast, flags);
}

void FloatInput::show() {
	ImGui::InputFloat(label.c_str(), &value, step, stepFast, "%g", flags);
}

void IntSlider::show() {
	ImGui::SliderInt(label.c_str(), &value, min, max, "%d", flags);
}

void FloatSlider::show() {
	ImGui::SliderFloat(label.c_str(), &value, min, max, "%g", flags);
}

void Button::show() {
	if (ImGui::Button(label.c_str(), size)) {
		if (OnClicked != nullptr) OnClicked();
	}
}
void ImageButton::show() {
	if (ImGui::ImageButton(image.asImTexture(), size)) {
		if (OnClicked != nullptr) OnClicked();
	}
}

void MenuItem::show() {
	static bool _selected = false;
	ImGui::MenuItem(label.c_str(), shortcut.c_str(), &_selected, enabled);
}

void Menu::addItem(MenuItem&& item) {
	m_menuItems.emplace_back(item);
}
void Menu::addItem(const MenuItem& item) {
	m_menuItems.emplace_back(item);
}
void Menu::addSeparator(Separator&& separator) {
	m_menuItems.emplace_back(separator);
}
void Menu::addSeparator(const Separator& separator) {
	m_menuItems.emplace_back(separator);
}

void Menu::show() {
	if (ImGui::BeginMenu(label.c_str(), enabled)) {
		for (auto& item : m_menuItems) {
			if (item.index() == 0) {
				std::get<0>(item).show();
			} else {
				std::get<1>(item).show();
			}
		}
		ImGui::EndMenu();
	}
}

void MenuBar::addMenu(Menu&& menu) {
	m_menus.emplace_back(menu);
}
void MenuBar::addMenu(const Menu& menu) {
	m_menus.emplace_back(menu);
}

void TabItem::show() {
	if (ImGui::BeginTabItem(label.c_str(), &isOpen, flags)) {
		if (content != nullptr) {
			content->show();
		}
		ImGui::EndTabItem();
	}
}

void MenuBar::show() {
	if (m_mainbar ? ImGui::BeginMainMenuBar() : ImGui::BeginMenuBar()) {
		for (auto& menu : m_menus) {
			menu.show();
		}
		if (m_mainbar) ImGui::EndMainMenuBar();
		else ImGui::EndMenuBar();
	}
}

void TabLayout::addTab(TabItem&& item) {
	m_tabs.emplace_back(item);
}
void TabLayout::addTab(const TabItem& item) {
	m_tabs.emplace_back(item);
}

void TabLayout::show() {
	if (ImGui::BeginTabBar(m_id, flags)) {
		for (auto& tab : m_tabs) {
			tab.show();
		}
		ImGui::EndTabBar();
	}
}

template<WidgetType T>
void ColumnLayout::addWidget(int column, T&& widget) {
	m_widgets.emplace_back(column, std::make_shared<IWidget>(widget));
}

template<WidgetType T>
void ColumnLayout::addWidget(int column, const T& widget) {
	m_widgets.emplace_back(column, std::make_shared<IWidget>(widget));
}

void ColumnLayout::show() {
	ImGui::Columns(m_columns);
	for (int i = 0; i < m_columns; i++) {
		for (auto& widget : m_widgets) {
			if (widget.column == i) {
				widget.widget->show();
			}
		}
		ImGui::NextColumn();
	}
}

TreeNode::TreeNode(const std::string& label, const std::initializer_list<TreeNode>& children): label(label) {
	nodes.reserve(children.size());
	for (auto& child : children) {
		nodes.push_back(child);
	}
}
TreeNode::TreeNode(const std::string& label, const std::vector<TreeNode>& children): label(label), nodes(children) { }
TreeNode::TreeNode(std::string&& label, const std::initializer_list<TreeNode>& children): label(std::move(label)) {
	nodes.reserve(children.size());
	for (auto& child : children) {
		nodes.push_back(child);
	}
}
TreeNode::TreeNode(std::string&& label, const std::vector<TreeNode>& children): label(std::move(label)), nodes(children) { }
TreeNode::TreeNode(const std::string& label, std::vector<TreeNode>&& children): label(label), nodes(std::move(children)) { }
TreeNode::TreeNode(std::string&& label, std::vector<TreeNode>&& children): label(std::move(label)), nodes(std::move(children)) { }

void TreeNode::show() {
	if (nodes.empty()) {
		if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			if (OnSelected != nullptr) OnSelected(*this);
		}
	} else {
		if (ImGui::TreeNode(label.c_str())) {
			if (OnSelected != nullptr) OnSelected(*this);
			for (auto child: nodes) {
				child.show();
			}
			ImGui::TreePop();
		}
	}
}


FreeTreeNode::FreeTreeNode(const std::filesystem::path& path, const std::initializer_list<FreeTreeNode>& children): path(path) {
	nodes.reserve(children.size());
	for (auto& child : children) {
		nodes.push_back(child);
	}
}
FreeTreeNode::FreeTreeNode(const std::filesystem::path& path, const std::vector<FreeTreeNode>& children): path(path), nodes(children) { }
FreeTreeNode::FreeTreeNode(std::filesystem::path&& path, const std::initializer_list<FreeTreeNode>& children): path(std::move(path)) {
	nodes.reserve(children.size());
	for (auto& child : children) {
		nodes.push_back(child);
	}
}
FreeTreeNode::FreeTreeNode(std::filesystem::path&& path, const std::vector<FreeTreeNode>& children): path(std::move(path)), nodes(children) { }
FreeTreeNode::FreeTreeNode(const std::filesystem::path& path, std::vector<FreeTreeNode>&& children): path(path), nodes(std::move(children)) { }
FreeTreeNode::FreeTreeNode(std::filesystem::path&& path, std::vector<FreeTreeNode>&& children): path(std::move(path)), nodes(std::move(children)) { }

void FreeTreeNode::show() {
	if (nodes.empty()) {
		Image img;
		if (FileIconProvider != nullptr) {
			FileIconProvider(path.extension().string());
		}
		if (img.id() == 0) {
			img = ImgFile;
		}

		ImGui::Image(img.asImTexture(), { 16, 16 }); ImGui::SameLine();
		if (ImGui::TreeNodeEx(path.filename().string().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			if (OnSelected != nullptr) OnSelected(*this);
		}
	} else {
		ImGui::Image(ImgFolder.asImTexture(), { 16, 16 }); ImGui::SameLine();
		if (ImGui::TreeNode(path.filename().string().c_str())) {
			if (OnSelected != nullptr) OnSelected(*this);
			for (auto child: nodes) {
				child.show();
			}
			ImGui::TreePop();
		}
	}
}

FreeTreeNode FreeTreeNode::BuildFromDirPath(const std::filesystem::path& rootPath) {
	FreeTreeNode root { rootPath, { } };
	for (const auto& path : std::filesystem::directory_iterator(rootPath)) {
		if (path.is_directory()) {
			root.nodes.push_back(BuildFromDirPath(path.path()));
		} else {
			root.nodes.push_back(FreeTreeNode { path.path(), { } });
		}
	}
	return root;
}

void ImEdGui_Init(const std::filesystem::path& basedir) {
	// Preload images

	if (std::filesystem::exists(basedir / "assets" / "folder.png")) {
		ImgFolder = Image(basedir / "assets" / "folder.png");
	}
	if (std::filesystem::exists(basedir / "assets" / "file.png")) {
		ImgFile = Image(basedir / "assets" / "file.png");
	}
}

BulletPoints::BulletPoints(const std::initializer_list<std::string>& items) {
	m_items.reserve(items.size());
	for (auto& item : items) {
		m_items.push_back(item);
	}
}

void BulletPoints::show() {
	for (const auto& item : m_items) {
		ImGui::BulletText("%s", item.c_str());
	}
}
