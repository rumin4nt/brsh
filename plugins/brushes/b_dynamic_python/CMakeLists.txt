cmake_minimum_required (VERSION 3.0)

project(b_dynamic_python)
set (b_dynamic_python_VERSION_MAJOR 0)
set (b_dynamic_python_VERSION_MINOR 0)
set (b_dynamic_python_VERSION_PATCH 1)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c

)


if(${OperatingSystem} MATCHES "linux")
	link_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/x86_64)
endif(${OperatingSystem} MATCHES "linux")


FIND_LIBRARY(PM_LIBRARY portaudio)

if(${OperatingSystem} MATCHES "macos")
	LINK_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/darwin )
endif(${OperatingSystem} MATCHES "macos")

add_library(b_dynamic_python SHARED ${sources})

target_include_directories (b_dynamic_python PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib/include)
target_include_directories (b_dynamic_python PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib)
target_include_directories (b_dynamic_python PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libs)

target_link_libraries(b_dynamic_python m)

#file(MAKE_DIRECTORY "/Library/Application Support/vblit/plugins/b_dynamic_python.plugin/")
#set directory "/Library/Application Support/vblit/plugins/b_dynamic_python.plugin"
# add_custom_target(b_dynamic_python_mkdir ALL COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_dynamic_python.plugin")
#add_custom_command(b_dynamic_python PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_dynamic_python.plugin")
# execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_dynamic_python.plugin")
add_custom_command(TARGET b_dynamic_python PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}/Library/Application\ Support/wash/plugins/b_dynamic_python.plugin)

if(${OperatingSystem} MATCHES "macos")

add_custom_command(
				TARGET b_dynamic_python POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy
								${CMAKE_CURRENT_BINARY_DIR}/libb_dynamic_python.dylib
		$ENV{HOME}/Library/Application\ Support/wash/brushes/b_dynamic_python.plugin/libb_dynamic_python.plugin.dylib)


#add_custom_command(
#        TARGET b_dynamic_python POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_CURRENT_BINARY_DIR}/libb_dynamic_python.dylib
#		$ENV{HOME}/Library/Application\ Support/comms/plugins/b_dynamic_python.plugin/libb_dynamic_python.plugin.dylib)

endif(${OperatingSystem} MATCHES "macos")
