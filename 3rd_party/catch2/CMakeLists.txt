cmake_minimum_required(VERSION 3.0)

project(catch2)

# See https://github.com/wtdcode/tun2socks/blob/master/tun2socks-core/3rdparty/catch2/CMakeLists.txt
# Prepare "Catch2" library for other executables
add_library(Catch2 INTERFACE)
add_library(Catch2::Test ALIAS Catch2)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})