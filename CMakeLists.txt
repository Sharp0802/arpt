cmake_minimum_required(VERSION 3.27)
project(arpt)

set(VERSION 0.0.1)

if (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    message(FATAL_ERROR "MSVC or Clang is required to compile ARPT.")
endif ()

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
target_compile_options(arpt PRIVATE -DEBUG:NULL -Zi)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(arpt PRIVATE -fno-rtti -fdeclspec -fms-extensions)
endif ()

if ((CMAKE_SYSTEM_NAME STREQUAL "Android") OR WIN32)
    target_link_directories(arpt PRIVATE dep)
endif ()

if (WIN32)
    target_link_libraries(arpt PRIVATE ws2_32 iphlpapi wpcap)
else ()
    target_link_libraries(arpt PRIVATE pcap)
endif ()
