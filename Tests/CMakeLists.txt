set(target MythicRuntimeTests)

add_executable(${target}
	main.test.cpp
)

target_link_libraries(${target} PRIVATE
	Runtime
	gtest
)
target_include_directories(${target} PRIVATE
						   ${CMAKE_SOURCE_DIR}/Runtime/Public
)


include(GoogleTest)
gtest_discover_tests(${target})