cmake_minimum_required(VERSION 3.6.2)

project(Elligator-2-Curve25519 VERSION 2.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_library(library SHARED main.c)

set_target_properties(
	library PROPERTIES
	OUTPUT_NAME elligator-2-curve25519
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/main.map
	LINK_FLAGS -Wl,-version-script="${CMAKE_CURRENT_SOURCE_DIR}/main.map"
)

target_link_libraries(library gmp)

add_executable(tests tests.c test-vectors.c main.c)

target_link_libraries(tests gmp -lcheck)

target_compile_definitions(tests PRIVATE WITH_TESTS)

add_custom_target(check ALL tests)

configure_file(Doxyfile.in Doxyfile)

add_custom_command(
	OUTPUT html
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/html
	COMMAND doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	DEPENDS Doxyfile.in index.md elligator-2-curve25519.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(documentation ALL DEPENDS html)

include(GNUInstallDirs)

install(TARGETS library LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES elligator-2-curve25519.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR})
