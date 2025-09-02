include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(glm)