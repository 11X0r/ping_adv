cmake_minimum_required(VERSION 3.10)
project(ping_adv VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -O2
)

file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/src/core/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)