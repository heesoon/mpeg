set(BIN_NAME tsfilter)

set(SRC_LIST
	${CMAKE_SOURCE_DIR}/tsfilterDebug.cpp
	${CMAKE_SOURCE_DIR}/tsreader.cpp
	${CMAKE_SOURCE_DIR}/tsfilter.cpp
	${CMAKE_SOURCE_DIR}/pat.cpp
	${CMAKE_SOURCE_DIR}/pmt.cpp
	${CMAKE_SOURCE_DIR}/tsfilterManager.cpp
  	${CMAKE_SOURCE_DIR}/main.cpp
)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/json_spirit
)

# find requred packagesa
find_package(Threads)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread filesystem REQUIRED REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif (Boost_FOUND)

# build components
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CXX_DEBUG_FLAGS "-g")

#add_definitions(-std=c++11 -g -pthread)
add_definitions(-std=c++1z -g -pthread)

# set library search path
#link_directories(${CMAKE_SOURCE_DIR}/libs)
link_directories(${CMAKE_SOURCE_DIR} 
				${CMAKE_SOURCE_DIR}/json_spirit
)

set(LIB_LIST
  	pthread
  	json_spirit
	${Boost_LIBRARIES}
  	${CMAKE_THREAD_LIBS_INIT}
)

add_executable(${BIN_NAME} ${SRC_LIST})

target_link_libraries(${BIN_NAME} ${LIB_LIST})
