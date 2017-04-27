# set library name
set(STATIC_LIBRARY_NAME mpeg-static)
set(DYNAMIC_LIBRARY_NAME mpeg)

# set installation path
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/libs)
set(INSTALL_LIB_PATH ${CMAKE_SOURCE_DIR}/libs)
set(INSTALL_INC_PATH ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CXX_DEBUG_FLAGS "-g")

add_definitions(-std=c++11 -g -pthread)

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mpeg_version.h.in
#  ${CMAKE_CURRENT_BINARY_DIR}/mpeg_version.h)

# set include path
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

# find requred packagesa
find_package(Threads)

# set dependent libary list
set(DEPENDENT_LIBRARY_LIST 
	pthread
)

# set source files built
set(SRCS
	${CMAKE_SOURCE_DIR}/src/sifilter.cpp
)

# install header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${INSTALL_INC_PATH}
	FILES_MATCHING PATTERN "*.hpp")

if (MPEG_STATIC)
add_library(${STATIC_LIBRARY_NAME} STATIC
	${SRCS}
)
#set_target_properties(${STATIC_LIBRARY_NAME} PROPERTIES OUTPUT_NAME "mpeg")
target_link_libraries(${STATIC_LIBRARY_NAME} ${DEPENDENT_LIBRARY_LIST})

#install(TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/lib${STATIC_LIBRARY_NAME}.a DESTINATION ${INSTALL_LIB_PATH}) 
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib${STATIC_LIBRARY_NAME}.a DESTINATION ${INSTALL_LIB_PATH})
endif()

if (MPEG_SHARED)
add_library(${DYNAMIC_LIBRARY_NAME} SHARED
	${SRCS}
)
#set_target_properties(${DYNAMIC_LIBRARY_NAME} PROPERTIES SUFFIX ".so.${PROJECT_VERSION}")
target_link_libraries(${DYNAMIC_LIBRARY_NAME} ${DEPENDENT_LIBRARY_LIST})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib${DYNAMIC_LIBRARY_NAME}.so DESTINATION ${INSTALL_LIB_PATH})
endif()
