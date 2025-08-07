install(CODE "message(STATUS \"[fmod] install fmod stuff ...\")")

# trying to configurations of installations
# if(TARGET fmod::fmod)
#     message()
# endif()

#CHECK exist install prefix

install(SCRIPT "cmake/test.cmake")

# include(GNUInstallDirs)

# it dosent work with IMPORTED LIB
# install(
#     TARGETS fmod
#     EXPORT fmod-targets
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLDEDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

#there is two option for imported lib (transform to INTERFACE LIBRARY or make config file manually)

get_target_property(fmod_lib_path fmod IMPORTED_IMPLIB)
message(STATUS "[fmod] install able lib : ${fmod_lib_path}")
get_target_property(fmod_dll_path fmod IMPORTED_LOCATION)
message(STATUS "[fmod] install dll : ${fmod_dll_path}")

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/fmod-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/fmod-config.cmake
    INSTALL_DESTINATION lib/cmake/fmod
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# TODO "include\" not to be realative
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES "${fmod_lib_path}" DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${fmod_dll_path}" DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fmod-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fmod
)