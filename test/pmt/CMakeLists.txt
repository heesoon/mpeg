cmake_minimum_required(VERSION 2.8)

project(test)

MESSAGE( STATUS "CMAKE_SOURCE_DIR : " ${CMAKE_SOURCE_DIR})
MESSAGE( STATUS "CMAKE_LIBRARY_PATH : " ${CMAKE_LIBRARY_PATH})

# set variables
set(BIN_NAME test)
set(SRCS 	
		 ${PROJECT_SOURCE_DIR}/section.cpp
		 ${PROJECT_SOURCE_DIR}/pat.cpp
		 ${PROJECT_SOURCE_DIR}/pmt.cpp
		 ${PROJECT_SOURCE_DIR}/test.cpp
)

set(LIB_LIST 
			pthread
)

# add definition
add_definitions(-std=c++0x)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# create executable
add_executable(${BIN_NAME} ${SRCS})

target_link_libraries(${BIN_NAME}
						${LIB_LIST}
)
