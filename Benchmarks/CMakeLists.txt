set(target Benchmarks)

add_executable(${target} main.benchmarks.cpp)

target_link_libraries(${target} PRIVATE benchmark::benchmark benchmark::benchmark_main)

target_link_libraries(${target} PRIVATE
	Runtime
	benchmark::benchmark 
)
target_include_directories(${target} PRIVATE
						   ${CMAKE_SOURCE_DIR}/Runtime/Public
)