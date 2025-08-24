set(target ${CMAKE_PROJECT_NAME})

add_executable(${target}
	main.cpp
)

target_link_libraries(${target} PRIVATE Runtime)
target_include_directories(${target} PRIVATE
						   ${CMAKE_SOURCE_DIR}/Runtime/Public
)