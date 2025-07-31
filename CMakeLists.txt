cmake_minimum_required(VERSION 3.22)

project(fmod LANGUAGES C CXX)

option(ENABLE_EXAMPLE "enable examples" ON)

add_library(fmod SHARED IMPORTED)
add_library(fmod::fmod ALIAS fmod)

# if(WIN32 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
#     set(fmod_lib "${CMAKE_STATIC_LIBRARY_PREFIX}fmod${CMAKE_STATIC_LIBRARY_SUFFIX}")
# endif()
set(fmod_lib "fmod64_vc.lib")
message(STATUS "[build] fmod implib : ${fmod_lib}")

set_target_properties(fmod PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod64.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/${fmod_lib}"
)

target_include_directories(fmod INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


if(ENABLE_EXAMPLE)
    add_subdirectory(exmaple)
endif()

message(STATUS "[build] ${CMAKE_CXX_COMPILER_ID}")