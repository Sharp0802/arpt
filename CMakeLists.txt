cmake_minimum_required(VERSION 3.27)
project(arpt)

set(VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 26)
file(GLOB_RECURSE SOURCES src/*.cxx)

file(GLOB_RECURSE HEADERS inc/*.h)
foreach (HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/inc" "${HEADER}")
    set(INCLUDES "${INCLUDES}#include \"${HEADER_PATH}\"\n")
endforeach ()
string(TIMESTAMP TIMESTAMP "%Y%m%d")
configure_file(module.h.in module.h)

add_executable(arpt ${HEADERS} ${SOURCES})
target_include_directories(arpt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_options(arpt PRIVATE -fno-rtti -fdeclspec -fms-extensions)

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_link_directories(arpt PRIVATE dep)
endif ()
target_link_libraries(arpt PRIVATE pcap)
