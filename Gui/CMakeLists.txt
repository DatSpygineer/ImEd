cmake_minimum_required(VERSION 3.28)
project(ImEdGui)

find_package(OpenGL REQUIRED)
find_package(Glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(imgui)

add_library(ImEdGui imed_gui_layout.cpp imed_gui_texteditor.cpp imed_gui_types.cpp imed_gui_common.cpp)
target_include_directories(ImEdGui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(ImEdGui PUBLIC ImEdNDiag OpenGL::GL GLEW::GLEW glfw imgui imgui::glfw imgui::opengl3 fmt::fmt)
target_compile_definitions(ImEdGui PUBLIC -DIMED_GUI=1)