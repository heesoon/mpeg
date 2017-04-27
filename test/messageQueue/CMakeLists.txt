set(BIN_NAME testMessageQeueue)

set(SRC_LIST
  ${CMAKE_SOURCE_DIR}/test/messageQueue/testMessageQeueue.cpp
)

# add include files 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
)

# find requred packagesa
find_package(Threads)

# build components
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CXX_DEBUG_FLAGS "-g")

add_definitions(-std=c++11 -g -pthread)

set(LIB_LIST
  pthread
  ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(${BIN_NAME} ${SRC_LIST})

target_link_libraries(${BIN_NAME} ${LIB_LIST})
