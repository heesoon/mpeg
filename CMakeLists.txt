cmake_minimum_required(VERSION 2.8)

project(mpeg)

set(PROJECT_NAME "mpeg")
set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

option(MPEG_TESTS "Build mpeg tests and enable testing" ON)
option(MPEG_STATIC "Build static libmpeg library" ON)
option(MPEG_SHARED "Build shared libmpeg library" ON)

add_subdirectory(src)
if(MPEG_TESTS)
add_subdirectory(test)
endif()
