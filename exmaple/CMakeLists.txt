
add_executable(sample main.cpp)

set_target_properties(sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
)

target_link_libraries(sample PRIVATE fmod::fmod)

add_custom_target(copy_dll ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_PROPERTY:fmod::fmod,IMPORTED_LOCATION>
        $<TARGET_FILE_DIR:sample>
)

add_dependencies(sample copy_dll)