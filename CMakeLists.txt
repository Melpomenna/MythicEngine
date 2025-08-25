cmake_minimum_required(VERSION 3.25.1...4.0)

enable_testing()

project(MythicEngine VERSION 1.0.0 LANGUAGES CXX ASM)


set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}:${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

include(CompileOptions)
SetCompileOptions()

add_subdirectory(External)
add_subdirectory(Runtime)
add_subdirectory(App)

if (MYTHIC_ENGINE_BUILD_TESTS)
	add_subdirectory(Tests)
endif()

if (MYTHIC_ENGINE_BUILD_BENCHMARKS)
	add_subdirectory(Benchmarks)
endif()