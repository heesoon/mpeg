# set application name
set(BIN_NAME app)

# set include path
include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include
)

# find requred packagesa
find_package(Threads)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread timer chrono filesystem REQUIRED REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif (Boost_FOUND)

# build components
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CXX_DEBUG_FLAGS "-g")

add_definitions(-std=c++11 -g -pthread)

# set library search path
link_directories(${CMAKE_SOURCE_DIR}/libs)

# set dependent libary list
set(DEPENDENT_LIBRARY_LIST 
	pthread
	mpeg
	${Boost_LIBRARIES}
)

# set source files built
set(SRCS
	${CMAKE_SOURCE_DIR}/app/main.cpp
)

add_executable(${BIN_NAME} ${SRCS})

target_link_libraries(${BIN_NAME} ${DEPENDENT_LIBRARY_LIST})
