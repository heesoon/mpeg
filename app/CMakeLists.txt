# set application name
set(BIN_NAME app)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CXX_DEBUG_FLAGS "-g")

add_definitions(-std=c++11 -g -pthread)

# set include path
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# find requred packagesa
find_package(Threads)

# set library search path
link_directories(${CMAKE_SOURCE_DIR}/libs)

# set dependent libary list
set(DEPENDENT_LIBRARY_LIST 
	pthread
	mpeg
)

# set source files built
set(SRCS
	${CMAKE_SOURCE_DIR}/app/main.cpp
)

add_executable(${BIN_NAME} ${SRCS})

target_link_libraries(${BIN_NAME} ${DEPENDENT_LIBRARY_LIST})