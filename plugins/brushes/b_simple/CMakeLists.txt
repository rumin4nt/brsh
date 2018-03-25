cmake_minimum_required (VERSION 3.0)

project(b_simple)
set (b_simple_VERSION_MAJOR 0)
set (b_simple_VERSION_MINOR 0)
set (b_simple_VERSION_PATCH 1)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c

)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	 # Mac OS X specific code
	 SET(OperatingSystem "macos")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		# Linux specific code
		SET(OperatingSystem "linux")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(WIN32)
		SET(OperatingSystem "windows")
ENDIF()


if(${OperatingSystem} MATCHES "linux")
	link_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/x86_64)
endif(${OperatingSystem} MATCHES "linux")


FIND_LIBRARY(PM_LIBRARY portaudio)

if(${OperatingSystem} MATCHES "macos")
	LINK_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/darwin )
endif(${OperatingSystem} MATCHES "macos")

add_library(b_simple SHARED ${sources})

target_include_directories (b_simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib/include)
target_include_directories (b_simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib)
target_include_directories (b_simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs)
target_include_directories (b_simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libs)
target_link_libraries(b_simple m)

#file(MAKE_DIRECTORY "/Library/Application Support/vblit/plugins/b_simple.plugin/")
#set directory "/Library/Application Support/vblit/plugins/b_simple.plugin"
# add_custom_target(b_simple_mkdir ALL COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_simple.plugin")
#add_custom_command(b_simple PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_simple.plugin")
# execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/b_simple.plugin")
add_custom_command(TARGET b_simple PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}/Library/Application\ Support/wash/plugins/b_simple.plugin)

if(${OperatingSystem} MATCHES "macos")

add_custom_command(
				TARGET b_simple POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy
								${CMAKE_CURRENT_BINARY_DIR}/libb_simple.dylib
		$ENV{HOME}/Library/Application\ Support/wash/brushes/b_simple.plugin/libb_simple.plugin.dylib)


#add_custom_command(
#        TARGET b_simple POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_CURRENT_BINARY_DIR}/libb_simple.dylib
#		$ENV{HOME}/Library/Application\ Support/comms/plugins/b_simple.plugin/libb_simple.plugin.dylib)

endif(${OperatingSystem} MATCHES "macos")
