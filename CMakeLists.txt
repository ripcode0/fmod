cmake_minimum_required(VERSION 3.22)

project(fmod LANGUAGES C CXX)

add_library(fmod SHARED IMPORTED)
add_library(fmod::fmod ALIAS fmod)

set_target_properties(fmod PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod64.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod64_vc.lib"
)

target_include_directories(fmod INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(myapp srcs/main.cpp)
set_target_properties(myapp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
)

target_link_libraries(myapp PRIVATE fmod::fmod)

message(STATUS "[build] ${CMAKE_STATIC_LIBRARY_PREFIX}")
message(STATUS "[build] ${CMAKE_STATIC_LIBRARY_SUFFIX}")

add_custom_target(copy_dll ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod64.dll
        $<TARGET_FILE_DIR:myapp>
)

add_dependencies(myapp copy_dll)