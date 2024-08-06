cmake_minimum_required(VERSION 3.28)
project(ImEdCore)

find_package(Lua REQUIRED)

add_subdirectory(../thirdparty/sol2 sol2)

add_library(ImEdCore imed_common.cpp imed_lua.cpp)
target_include_directories(ImEdCore PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${LUA_INCLUDE_DIR})
target_link_libraries(ImEdCore PUBLIC ${LUA_LIBRARIES} ImEdGui ImEdNDiag fmt::fmt sol2::sol2)
target_compile_definitions(ImEdCore PUBLIC -DIMED_CORE=1)