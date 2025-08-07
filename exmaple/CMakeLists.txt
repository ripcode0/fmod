
if(WIN32)
    set(sample_srcs sample_win32.cpp)
else()
    set(sample_srcs sample_linux.cpp)
endif()

add_executable(sample ${sample_srcs})

set_target_properties(sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/Debug
)
if(MSVC)
    set_target_properties(sample PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )
endif()

target_link_libraries(sample PRIVATE fmod::fmod)

get_target_property(fmod_dll fmod::fmod IMPORTED_LOCATION)

# add_custom_target(print_dll ALL
#     COMMAND ${CMAKE_COMMAND} -E echo found the ${fmod_dll}
# )

# add_dependencies(sample print_dll)

if(WIN32)
    add_custom_command(
        TARGET sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PROPERTY:fmod::fmod,IMPORTED_LOCATION>
            $<TARGET_FILE_DIR:sample>
            COMMENT "copying dlls"
        )
endif()

file(GLOB resource ${CMAKE_CURRENT_SOURCE_DIR}/data/*.mp3)


foreach(mp3 IN LISTS resource)
    add_custom_command(
        TARGET sample
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${mp3}
            $<TARGET_FILE_DIR:sample>
    )
endforeach()


# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/data.stamp
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             ${resource}
#             $<TARGET_FILE_DIR:sample>
#     COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/data.stamp
#     DEPENDS ${resource}
#     COMMENT "copying resource"
#     )

# add_custom_target(copy_resource ALL
#     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/data.stamp
# )

# add_dependencies(sample copy_resource)