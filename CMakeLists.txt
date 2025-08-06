cmake_minimum_required(VERSION 3.22)

project(fmod LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(fmod_standalone TRUE)
    message(STATUS "[fmod] Standalone build of FMOD")
else()
    set(fmod_standalone FALSE)
endif()

if(WIN32 AND MSVC)
    set(fmod_lib "fmod64_vc${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(fmod_dll "fmod64${CMAKE_SHARED_LIBRARY_SUFFIX}")
elseif(WIN32 AND ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") # windows with gnu tripple
    set(fmod_lib "fmod64_vc.lib")
    #set(fmod_lib "libfmod.a") # not tested
    set(fmod_dll "fmod64.dll")
elseif(UNIX AND NOT APPLE) # linux
    set(fmod_so_name "libfmod.so.10")
    set(fmod_so "libfmod.so.10.20")
    message(${fmod_so_name})
endif()

option(ENABLE_EXAMPLE "enable examples" ON)
option(ENABLE_INSTALL "enable to installation" ON)

add_library(fmod SHARED IMPORTED GLOBAL)
add_library(fmod::fmod ALIAS fmod)


message(STATUS "[fmod] fmod implib : ${fmod_lib}")
message(STATUS "[fmod] fmod shared : ${fmod_dll}")

if(WIN32)
    set_target_properties(fmod PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/${fmod_dll}"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/${fmod_lib}"
    )
else()
    set_target_properties(fmod PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/${fmod_so}"
        IMPORTED_SONAME "${fmod_so_name}"
    )
endif()

target_include_directories(fmod INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(ENABLE_EXAMPLE)
    add_subdirectory(exmaple)
endif()

message(STATUS "[build] Compiler : ${CMAKE_CXX_COMPILER_ID}")

if(ENABLE_INSTALL)
    include(cmake/fmod_install.cmake)
endif()