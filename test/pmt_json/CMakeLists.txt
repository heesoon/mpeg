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
			${Boost_LIBRARIES}
      		json_spirit			
)

# add definition
add_definitions(-std=c++0x)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/json_spirit)

# add external library path
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/json_spirit)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.54 REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif (Boost_FOUND)

# create executable
add_executable(${BIN_NAME} ${SRCS})

target_link_libraries(${BIN_NAME}
						${LIB_LIST}
)
