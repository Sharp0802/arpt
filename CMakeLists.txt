cmake_minimum_required(VERSION 3.27)
project(arpt)

set(VERSION 0.0.1)

if (BITS STREQUAL "32")
    set(WPCAP_NAME wpcap.32)
elseif (BITS STREQUAL "64")
    set(WPCAP_NAME wpcap.64)
else ()
    if (WIN32)
        message(FATAL_ERROR "BITS(32, 64) and PLATFORM(x86, ARM) must be specified.")
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ARPT_COMPILE_OPTIONS -fno-rtti -fdeclspec -fms-extensions)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(ARPT_COMPILE_OPTIONS -DEBUG:FULL -Zi)
else ()
    message(FATAL_ERROR "MSVC or Clang is required to compile ARPT.")
endif ()

if (WIN32)
    file (
        DOWNLOAD
        https://npcap.com/dist/npcap-sdk-1.13.zip
        "${CMAKE_BINARY_DIR}/npcap.zip"
        SHOW_PROGRESS
    )
    file (ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/npcap.zip" DESTINATION "${CMAKE_BINARY_DIR}/npcap")
    if (BITS EQUAL 32)
        if (PLATFORM STREQUAL "ARM")
            message(FATAL_ERROR "Windows with ARM32 is not supported.")
        else ()
            set(ARPT_LINK_DIRECTORIES "${CMAKE_BINARY_DIR}/npcap/Lib/")
        endif ()
    else ()
        if (PLATFORM STREQUAL "ARM")
            set(ARPT_LINK_DIRECTORIES "${CMAKE_BINARY_DIR}/npcap/Lib/ARM64/")
        else ()
            set(ARPT_LINK_DIRECTORIES "${CMAKE_BINARY_DIR}/npcap/Lib/x64/")
        endif ()
    endif ()

    set(ARPT_LINK_LIBRARIES ws2_32 iphlpapi wpcap)
else ()
    set(ARPT_LINK_LIBRARIES pcap)
endif ()

if ((CMAKE_SYSTEM_NAME STREQUAL "Android"))
    set(ARPT_LINK_DIRECTORIES dep)
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
target_compile_options(arpt PRIVATE ${ARPT_COMPILE_OPTIONS})
target_link_directories(arpt PRIVATE ${ARPT_LINK_DIRECTORIES})
target_link_libraries(arpt PRIVATE ${ARPT_LINK_LIBRARIES})
