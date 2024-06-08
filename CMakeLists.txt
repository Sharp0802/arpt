cmake_minimum_required(VERSION 3.27)
project(arpt)

#
# USE_NETLINK   : ON if netlink should be used to resolving gateways; otherwise, OFF.
#                 Default: ON; OFF(Android)
#

#################################################
# Versioning                                    #
#################################################
set(VERSION 0.0.1)

#################################################
# Compile Option Configurations                 #
#################################################
set(CMAKE_CXX_STANDARD 26)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ARPT_COMPILE_OPTIONS -fno-rtti -fdeclspec -fms-extensions -Wno-microsoft-goto)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(ARPT_COMPILE_OPTIONS /GR-)
else ()
    message(FATAL_ERROR "MSVC or Clang is required to compile ARPT.")
endif ()

#################################################
# Platform-Dependent Configurations             #
#################################################
if (WIN32)
    #################################################
    # Link Configurations                           #
    #################################################
    set(ARPT_LINK_LIBRARIES ws2_32 iphlpapi)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    #################################################
    # Compile Option Configurations                 #
    #################################################
    if (USE_NETLINK)
        message(WARNING "Netlink is not supported on Android; An option 'USE_NETLINK' will be ignored.")
    endif ()
    set(ARPT_DEFINITIONS ${ARPT_DEFINITIONS} -DUSE_NETLINK=0)
else ()
    #################################################
    # Compile Option Configurations                 #
    #################################################
    if (NOT EXISTS USE_NETLINK)
        message(STATUS "'USE_NETLINK' defaults to ON")
        set(USE_NETLINK ON)
    endif ()
    if (USE_NETLINK)
        set(USE_NETLINK_DEF 1)
    else ()
        set(USE_NETLINK_DEF 0)
    endif ()
    set(ARPT_DEFINITIONS ${ARPT_DEFINITIONS} -DUSE_NETLINK=${USE_NETLINK_DEF})

endif ()

#################################################
# File Configurations                           #
#################################################
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")

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
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
target_compile_definitions(arpt PRIVATE ${ARPT_DEFINITIONS})
target_precompile_headers(arpt PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc/pch.h")
target_compile_options(arpt PRIVATE ${ARPT_COMPILE_OPTIONS})
target_link_libraries(arpt PRIVATE ${ARPT_LINK_LIBRARIES})
