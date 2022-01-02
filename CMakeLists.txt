cmake_minimum_required(VERSION 3.3)
project(main LANGUAGES CXX)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/external/glad/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/external/glfw-3.3.6/include")

FILE(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB GLAD "${PROJECT_SOURCE_DIR}/external/glad/src/*.cpp")

link_directories(
    "${PROJECT_SOURCE_DIR}/external/glfw-3.3.6/bin"
)

add_executable(main ${SOURCES}
                    ${GLAD}
                    )

target_link_libraries(main 
                    glfw3
                    )