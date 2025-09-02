set(target MythicRuntime)

set(RUNTIME_PUBLIC_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Public)
set(RUNTIME_PRIVATE_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Private)

set(ALGORITHMS_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Algorithms/CircleBuffer.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Algorithms/IntrusiveNode.h
)

set(WINDOWS_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Windows/System.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Windows/System.cpp
)

set(MEMORY_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Memory/MimallocHelper.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Memory/MallocHelper.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Memory/MallocHelper.cpp
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Memory/MimallocHelper.cpp
)

set(LOG_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Log/QuillLoggerHelper.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Log/LogManager.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Log/Loggers.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Log/QuillLoggerHelper.cpp
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Log/LogManager.cpp
)

set(SYSTEM_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/System/SystemHelper.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/System/SystemHelper.cpp
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/System/SystemEnumerations.h
)

set(PARALLEL_SOURCES_FILES
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/CpuMaskHelper.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Parallel/CpuMaskHelper.cpp
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/ThreadHelper.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Parallel/ThreadHelper.cpp
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/ThreadPool.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/Spinlock.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/BoundingMPMCQueue.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Parallel/UnboundingMPMCQueue.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/Parallel/Spinlock.cpp
)

set(RUNTIME_CORE_SOURCES
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/MythicRuntime.cpp
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/MythicRuntime.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/Config.h
	${RUNTIME_PRIVATE_INCLUDE_DIRECTORY}/CoreRuntime.cpp
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/CoreRuntime.h
	${RUNTIME_PUBLIC_INCLUDE_DIRECTORY}/StaticString.h
)

add_library(${target} SHARED
	${RUNTIME_CORE_SOURCES}
	${PARALLEL_SOURCES_FILES}
	${SYSTEM_SOURCES_FILES}
	${LOG_SOURCES_FILES}
	${MEMORY_SOURCES_FILES}
	${WINDOWS_SOURCES_FILES}
	${ALGORITHMS_SOURCES_FILES}
)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)

target_link_libraries(${target} PRIVATE 
						quill
						glm
						glfw
						OpenAL::OpenAL
						unofficial::omniverse-physx-sdk::sdk
)

target_link_libraries(${target} PUBLIC
						mimalloc
)

target_include_directories(${target} PRIVATE 
						   ${CMAKE_CURRENT_SOURCE_DIR}/Public
						   ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_compile_definitions(${target} PRIVATE
						   RUNTIME_EXPORT
)

if(TARGET unofficial::omniverse-physx-sdk::gpu-library)
    if(UNIX)
        # Add rpath setting to find .so libraries on unix based systems
        set_target_properties(${target} PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN"
        )
    endif()
    add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-library>
                    $<TARGET_FILE_DIR:${target}>)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-device-library>
                        $<TARGET_FILE_DIR:${target}>)
    endif()
else()
    message(WARNING "GPU acceleration library target not defined - GPU acceleration will NOT be available!")
endif()