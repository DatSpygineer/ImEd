cmake_minimum_required(VERSION 3.28)
project(ImEd)

if (WIN32)
	include(vcpkg.cmake)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

find_package(Lua REQUIRED)

add_subdirectory(Core)
add_subdirectory(Gui)
add_subdirectory(NativeDialogues)
add_subdirectory(thirdparty/fmt)

add_executable(ImEd main.cpp)
target_link_libraries(ImEd PRIVATE ImEdCore)

set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/assets")
set(DEST_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_command(
	OUTPUT "${DEST_DIR}"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}" "${DEST_DIR}"
	DEPENDS "${SOURCE_DIR}"
	COMMENT "Copying assets to the output directory."
)

add_custom_target(copy_data ALL DEPENDS "${DEST_DIR}")
add_dependencies(ImEd copy_data)