cmake_minimum_required(VERSION 2.8)

option(TEST_MESSAGEQUEUE "Build messageQueue and enable messageQueue testing" ON)
option(TEST_FILTER "Build filter and enable messageQueue filter" ON)

if(TEST_MESSAGEQUEUE)
add_subdirectory(messageQueue)
endif()

if(TEST_FILTER)
add_subdirectory(filter)
endif()
