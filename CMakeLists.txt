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

if (CMAKE_BUILD_ON_CI)
    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        if(WIN32)
            include(InstallRequiredSystemLibraries)
    
            foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
                get_filename_component(lib_name ${lib} NAME)
                configure_file(${lib} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${lib_name} COPYONLY)
            endforeach()
    
            if(CMAKE_INSTALL_UCRT_LIBRARIES)
                foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS})
                    get_filename_component(lib_name ${lib} NAME)
                    configure_file(${lib} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${lib_name} COPYONLY)
                endforeach()
            endif()
        endif()
    endif()
endif()