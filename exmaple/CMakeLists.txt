
add_executable(sample main.cpp)

set_target_properties(sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
)

target_link_libraries(sample PRIVATE fmod::fmod)

# add_custom_target(copy_dll ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_PROPERTY:fmod::fmod,IMPORTED_LOCATION>
#         $<TARGET_FILE_DIR:sample>
# )

# add_dependencies(sample copy_dll)

get_target_property(fmod_dll fmod::fmod IMPORTED_LOCATION)

# add_custom_target(print_dll ALL
#     COMMAND ${CMAKE_COMMAND} -E echo found the ${fmod_dll}
# )

# add_dependencies(sample print_dll)

add_custom_command(
    TARGET sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_PROPERTY:fmod::fmod,IMPORTED_LOCATION>
        $<TARGET_FILE_DIR:sample>
        COMMENT "copying dlls"
    )