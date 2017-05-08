cmake_minimum_required(VERSION 2.8)

option(TEST_MESSAGEQUEUE "Build messageQueue and enable messageQueue testing" ON)
option(TEST_FILTER "Build filter and enable filter testing" ON)
option(TEST_IPTUNER "Build filter and enable iptuner testing" ON)

if(TEST_MESSAGEQUEUE)
add_subdirectory(messageQueue)
endif()

if(TEST_FILTER)
add_subdirectory(filter)
endif()

if(TEST_IPTUNER)
add_subdirectory(iptuner)
endif()
