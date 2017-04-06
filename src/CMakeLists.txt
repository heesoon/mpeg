set(LIBRARY "libmpeg")
set(STATICLIBRARY "libmpeg_static")
set(HEADERS
)
set(LIBRARY_SOURCES
	sifilter.cpp
	messageQueue.cpp
	${HEADERS}	
)

set(PROGRAM "mpeg")
set(PROGRAM_SOURCES
	main.cpp
)

include_directories(. ${CMAKE_CURRENT_BINARY_DIR})
include_directories(
#  ${PROJECT_BINARY_DIR}/extensions
)

add_executable(${PROGRAM} ${PROGRAM_SOURCES})
#add_compiler_export_flags()

target_link_libraries(${PROGRAM} libmpeg_static)

#add_dependencies(${PROGRAM} libmpeg-extensions_static)
target_link_libraries(${PROGRAM} libmpeg_static)

# Disable the PUBLIC declarations when compiling the executable:
set_target_properties(${PROGRAM} PROPERTIES
  COMPILE_FLAGS "-DCMARK_STATIC_DEFINE -DCMARKEXTENSIONS_STATIC_DEFINE")

# Check integrity of node structure when compiled as debug:
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCMARK_DEBUG_NODES -DDEBUG")
set(CMAKE_LINKER_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG}")

set(CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS_RELEASE} -pg")
set(CMAKE_LINKER_PROFILE "${CMAKE_LINKER_FLAGS_RELEASE} -pg")

add_definitions(-DLIBDIR=\"${CMAKE_BINARY_DIR}\")

if (${CMAKE_VERSION} VERSION_GREATER "1.8")
  set(CMAKE_C_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
elseif(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
endif ()

if (MPEG_SHARED)
  add_library(${LIBRARY} SHARED ${LIBRARY_SOURCES})
  # Include minor version and patch level in soname for now.
  set_target_properties(${LIBRARY} PROPERTIES
  OUTPUT_NAME "mpeg"
  SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
  VERSION ${PROJECT_VERSION})

  set_property(TARGET ${LIBRARY}
    APPEND PROPERTY MACOSX_RPATH true)

  # Avoid name clash between PROGRAM and LIBRARY pdb files.
  set_target_properties(${LIBRARY} PROPERTIES PDB_NAME mpeg_dll)

  generate_export_header(${LIBRARY}
    BASE_NAME ${PROJECT_NAME})

  list(APPEND MPEG_INSTALL ${LIBRARY})
endif()

if (MPEG_STATIC)
  add_library(${STATICLIBRARY} STATIC ${LIBRARY_SOURCES})
  set_target_properties(${STATICLIBRARY} PROPERTIES
  COMPILE_FLAGS -DMPEG_STATIC_DEFINE
  POSITION_INDEPENDENT_CODE ON)

  if (MSVC)
  set_target_properties(${STATICLIBRARY} PROPERTIES
    OUTPUT_NAME "mpeg_static"
    VERSION ${PROJECT_VERSION})
  else()
  set_target_properties(${STATICLIBRARY} PROPERTIES
    OUTPUT_NAME "mpeg"
    VERSION ${PROJECT_VERSION})
endif(MSVC)

target_link_libraries(mpeg ${CMAKE_DL_LIBS})

  list(APPEND MPEG_INSTALL ${STATICLIBRARY})
endif()
set_property(TARGET ${LIBRARY}
  APPEND PROPERTY MACOSX_RPATH true)

# Avoid name clash between PROGRAM and LIBRARY pdb files.
set_target_properties(${LIBRARY} PROPERTIES PDB_NAME mpeg_dll)

#generate_export_header(${LIBRARY}
#    BASE_NAME ${PROJECT_NAME})

if (MSVC)
  set_property(TARGET ${PROGRAM}
    APPEND PROPERTY LINK_FLAGS /INCREMENTAL:NO)
endif(MSVC)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)

install(TARGETS ${PROGRAM} ${MPEG_INSTALL}
  EXPORT mpeg
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib${LIB_SUFFIX}
  ARCHIVE DESTINATION lib${LIB_SUFFIX}
  )

if(MPEG_SHARED OR MPEG_STATIC)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libmpeg.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libmpeg.pc @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libmpeg.pc
    DESTINATION lib${LIB_SUFFIX}/pkgconfig)

  install(FILES
#  cmark.h
#  cmark_extension_api.h
#  ${CMAKE_CURRENT_BINARY_DIR}/cmark_export.h
#  ${CMAKE_CURRENT_BINARY_DIR}/cmark_version.h
  DESTINATION include
  )

  install(EXPORT cmark DESTINATION lib${LIB_SUFFIX}/cmake)
endif()