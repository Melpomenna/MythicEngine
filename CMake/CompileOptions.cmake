function(SetCompileOptions)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # MSVC (Windows)
        if(${CMAKE_BUILD_TYPE} MATCHES "Release")
            message(STATUS "Using Release compile options (MSVC)")
            add_link_options(/LTCG /CGTHREADS:8 /NOLOGO /MACHINE:x64 /DEBUG:FULL)
            add_compile_options(/MD /O2 /arch:AVX2 /favor:INTEL64 /bigobj /cgthreads8
                                /analyze /diagnostics:classic /GL /MP /Qpar /sdl
                                /std:c++latest /W4 /Zo /WX /Zc:__cplusplus
                                /external:anglebrackets /analyze:external-
                                /external:templates- /external:W0 /analyze:WX- /Zi)
            if (MYTHIC_ENGINE_USE_PROFILE)
                add_link_options(/USEPROFILE)
            else()
                add_link_options(/FASTGENPROFILE)
            endif()
        else()
            message(STATUS "Using Debug compile options (MSVC)")
            add_link_options(/CGTHREADS:2 /DEBUG /NOLOGO /MACHINE:x64)
            add_compile_options(/MDd /arch:AVX2 /favor:INTEL64 /bigobj /cgthreads2
                                /analyze /diagnostics:classic /fsanitize=address /MP
                                /RTCs /sdl /std:c++latest /Zi /W4 /Zf /WX
                                /Zc:__cplusplus /external:anglebrackets
                                /analyze:external- /external:templates- /external:W0
                                /analyze:WX-)
        endif()

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Clang macOS
        if(${CMAKE_BUILD_TYPE} MATCHES "Release")
            message(STATUS "Using Release compile options (Clang)")
            add_compile_options(-O3 -march=native -Wall -Wextra -Werror
                                -std=c++23 -fstrict-aliasing -flto)
            add_link_options(-flto)
        else()
            message(STATUS "Using Debug compile options (Clang)")
            add_compile_options(-O0 -g -Wall -Wextra -Werror
                                -std=c++23 -fsanitize=address -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address)
        endif()

    else()
        message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()
