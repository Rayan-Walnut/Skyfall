cmake_minimum_required(VERSION 3.14)
project(Skyfall LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for GLFW, GLAD and GLM
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.36
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)

FetchContent_MakeAvailable(glfw glad glm)

add_executable(Skyfall
  src/main.cpp
  src/Camera.cpp
  src/Player.cpp
)

target_include_directories(Skyfall PRIVATE
  ${glad_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Skyfall PRIVATE glfw)

if (MSVC)
  target_compile_definitions(Skyfall PRIVATE "GLFW_INCLUDE_NONE")
endif()

target_compile_definitions(Skyfall PRIVATE GL_SILENCE_DEPRECATION)

# Simple test target
add_executable(tests tests/test_basic.cpp)
add_test(NAME basic_tests COMMAND tests)

enable_testing()
