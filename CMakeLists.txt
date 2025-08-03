cmake_minimum_required(VERSION 3.22)

project(fmod LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(fmod_standalone TRUE)
    message(STATUS "[fmod] Standalone build of FMOD")
else()
    set(fmod_standalone FALSE)
endif()

if(WIN32)
    set(fmod_lib "fmod64_vc${CMAKE_STATIC_LIBRARY_SUFFIX}")
    set(fmod_dll "fmod64${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

option(ENABLE_EXAMPLE "enable examples" ON)

add_library(fmod SHARED IMPORTED)
add_library(fmod::fmod ALIAS fmod)

# if(WIN32 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
#     set(fmod_lib "${CMAKE_STATIC_LIBRARY_PREFIX}fmod${CMAKE_STATIC_LIBRARY_SUFFIX}")
# endif()

message(STATUS "[fmod] fmod implib : ${fmod_lib}")
message(STATUS "[fmod] fmod shared : ${fmod_dll}")

set_target_properties(fmod PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/${fmod_dll}"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/${fmod_lib}"
)

target_include_directories(fmod INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


if(ENABLE_EXAMPLE)
    add_subdirectory(exmaple)
endif()

message(STATUS "[build] ${CMAKE_CXX_COMPILER_ID}")