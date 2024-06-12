cmake_minimum_required(VERSION 3.10)
project(collisions)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

include_directories(${INCLUDE_DIR})

foreach(SOURCE ${SOURCES})
    get_filename_component(SOURCE_NAME ${SOURCE} NAME_WE)
    message(STATUS "Adding executable: ${SOURCE_NAME}")  
    add_executable(${SOURCE_NAME} ${SOURCE_DIR}/${SOURCE_NAME}.cpp)
    target_link_libraries(${SOURCE_NAME} raylib GL m pthread dl rt X11)
endforeach()
