add_subdirectory(math math)
add_subdirectory(geo geo)
add_subdirectory(geo_lookup geo_lookup)
add_subdirectory(systemlib systemlib)

set(PX4LIB_SOURCES 
    ${PX4LIB_LIB_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/px4lib.cpp
    ${PX4LIB_PUBLIC_INCLUDE_DIR}/px4lib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/px4lib_version.h
)

set(PX4LIB_INCLUDES ${PX4LIB_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})

set(PX4LIB_LIB_SOURCES  ${PX4LIB_LIB_SOURCES} PARENT_SCOPE)
set(PX4LIB_SOURCES  ${PX4LIB_SOURCES} PARENT_SCOPE)
set(PX4LIB_INCLUDES ${PX4LIB_INCLUDES} PARENT_SCOPE)
