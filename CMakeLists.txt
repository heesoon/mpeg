cmake_minimum_required(VERSION 2.8)

project(sifilter)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# find requred packagesa
find_package(Threads)

# build components

SET(CXX_LANGUAGE_FLAGS "-Wl,--no-as-needed -std=c++11 -pthread")
SET(CMAKE_EXE_LINKER_FLAGS "-pthread")
SET(CXX_DEBUG_FLAGS "-g")

# create executable
set(BIN_NAME test)
set(SRC_LIST
	${CMAKE_SOURCE_DIR}/sifilter.cpp
)

# add extern libs

# set libs list
#link_directories(/usr/local/lib)

set(LIB_LIST
	pthread
	${CMAKE_THREAD_LIBS_INIT}
)

#add_definitions(-std=c++0x -g -pthread)
add_definitions(-std=c++11 -g -pthread)

add_executable(${BIN_NAME} ${SRC_LIST})

target_link_libraries(${BIN_NAME} ${LIB_LIST})

#install example to /usr/lib
#install(FILES ${BIN_NAME} DESTINATION /usr/lib)

