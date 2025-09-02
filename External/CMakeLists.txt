add_subdirectory(mimalloc)

if (MYTHIC_ENGINE_BUILD_TESTS)
	add_subdirectory(googletest)
endif()

if (MYTHIC_ENGINE_BUILD_BENCHMARKS)
	add_subdirectory(benchmark)
endif()

add_subdirectory(glm)
add_subdirectory(quill)