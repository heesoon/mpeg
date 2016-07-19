cmake_minimum_required(VERSION 2.8)

project(test)

# add definition
add_definitions(-std=c++0x)

message(STATUS "CMAKE_SOURCE_DIR : 	" ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

# set variables
set(BIN_NAME test)
set(SRCS 
		${PROJECT_SOURCE_DIR}/test.cpp
)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

#find_package(Boost REQUIRED)
#find_package(Boost 1.54 REQUIRED system serialization)
find_package(Boost
             1.54.0
             COMPONENTS thread
                        system
                        serialization
                        log
                        log_setup
                        program_options
             REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
#	target_link_libraries( ${BIN_NAME} ${Boost_LIBRARIES})
endif (Boost_FOUND)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# create executable
add_executable(${BIN_NAME} ${SRCS})

target_link_libraries(${BIN_NAME}
	rt
	pthread
	${Boost_LIBRARIES}
)

