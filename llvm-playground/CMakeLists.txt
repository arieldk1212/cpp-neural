cmake_minimum_required(VERSION 3.24)
project(llvm-intro LANGUAGES CXX C
  DESCRIPTION "LLVM Introduction")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_PREFIX_PATH "/Users/$ENV{USER}/Library/vcpkg/installed/arm64-osx/share")
set(CMAKE_TOOLCHAIN_FILE "/Users/$ENV{USER}/Library/vcpkg/scripts/buildsystems/vcpkg.cmake")

enable_language(C)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND project_libraries
LLVMCore
)

list(APPEND sources
Main.cc
Parser/AST.CC
)

add_executable(Main ${sources})

target_link_libraries(Main PRIVATE ${project_libraries})