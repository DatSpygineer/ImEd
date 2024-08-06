cmake_minimum_required(VERSION 3.20)

project(ImEdNDiag)

if (NOT WIN32 AND NOT MINGW)
	find_package(GTK2 REQUIRED) # GTK is generally available for most desktops
endif()

add_library(ImEdNDiag STATIC ImEdNativeDiag.cpp)
target_include_directories(ImEdNDiag PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if (NOT WIN32 AND NOT MINGW)
	target_include_directories(ImEdNDiag PUBLIC ${GTK2_INCLUDE_DIRS})
	target_link_libraries(ImEdNDiag PUBLIC ${GTK2_LIBRARIES})
	target_compile_definitions(ImEdNDiag PUBLIC ${GTK2_DEFINITIONS})
endif()