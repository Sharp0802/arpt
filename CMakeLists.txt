cmake_minimum_required(VERSION 3.27)
project(arpt)

#################################################
# Versioning                                    #
#################################################
set(VERSION 0.0.1)
set(NPCAP_VERSION 1.13)

#################################################
# Compile Option Configurations                 #
#################################################
set(CMAKE_CXX_STANDARD 26)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ARPT_COMPILE_OPTIONS -fno-rtti -fdeclspec -fms-extensions)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(ARPT_COMPILE_OPTIONS -DEBUG:FULL -Zi)
else ()
    message(FATAL_ERROR "MSVC or Clang is required to compile ARPT.")
endif ()

#################################################
# Platform-Dependent Configurations             #
#################################################
if (WIN32)

    #################################################
    # NPCAP Configurations                          #
    #################################################
    file (
        DOWNLOAD
        "https://npcap.com/dist/npcap-sdk-${NPCAP_VERSION}.zip"
        "${CMAKE_BINARY_DIR}/npcap.zip"
    )
    file (ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/npcap.zip" DESTINATION "${CMAKE_BINARY_DIR}/npcap")

    if (NOT EXISTS CMAKE_SYSTEM_PROCESSOR)
        message(STATUS "CMAKE_SYSTEM_PROCESSOR is not set; defaults to '$ENV{PROCESSOR_ARCHITECTURE}'")
        set(CMAKE_SYSTEM_PROCESSOR "$ENV{PROCESSOR_ARCHITECTURE}")
    endif ()
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        set(PCAP_DIRECTORY "${CMAKE_BINARY_DIR}/npcap/Lib/x64/")
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "X86")
        set(PCAP_DIRECTORY "${CMAKE_BINARY_DIR}/npcap/Lib/")
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")
        set(PCAP_DIRECTORY "${CMAKE_BINARY_DIR}/npcap/Lib/ARM64/")
    else ()
        message(FATAL_ERROR "Architecture '${CMAKE_SYSTEM_PROCESSOR}' is not supported.")
    endif ()
    message(STATUS "NPCAP ${NPCAP_VERSION} is configured")

    #################################################
    # Link Configurations                           #
    #################################################
    set(ARPT_LINK_LIBRARIES ws2_32 iphlpapi wpcap)

else ()

    #################################################
    # Link Configurations                           #
    #################################################
    set(ARPT_LINK_LIBRARIES pcap)

endif ()

#################################################
# File Configurations                           #
#################################################
file(GLOB_RECURSE SOURCES src/*.cxx)
file(GLOB_RECURSE HEADERS inc/*.h)

#################################################
# Header Management Automation                  #
#################################################
foreach (HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/inc" "${HEADER}")
    set(INCLUDES "${INCLUDES}#include \"${HEADER_PATH}\"\n")
endforeach ()
string(TIMESTAMP TIMESTAMP "%Y%m%d")
configure_file(module.h.in module.h)

#################################################
# Commit Configurations                         #
#################################################
add_executable(arpt ${HEADERS} ${SOURCES})
target_include_directories(arpt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_options(arpt PRIVATE ${ARPT_COMPILE_OPTIONS})
target_link_directories(arpt PRIVATE ${PCAP_DIRECTORY})
target_link_libraries(arpt PRIVATE ${ARPT_LINK_LIBRARIES})
