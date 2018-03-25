cmake_minimum_required (VERSION 3.0)

project(a_jitter)
set (a_jitter_VERSION_MAJOR 0)
set (a_jitter_VERSION_MINOR 0)
set (a_jitter_VERSION_PATCH 1)

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
	# LINK_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/darwin )
endif(${OperatingSystem} MATCHES "macos")

add_library(a_jitter SHARED ${sources})

target_include_directories (a_jitter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib/include)
target_include_directories (a_jitter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib)
target_include_directories (a_jitter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libs)

target_link_libraries(a_jitter m)

#file(MAKE_DIRECTORY "/Library/Application Support/vblit/plugins/a_jitter.plugin/")
#set directory "/Library/Application Support/vblit/plugins/a_jitter.plugin"
# add_custom_target(a_jitter_mkdir ALL COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/a_jitter.plugin")
#add_custom_command(a_jitter PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/a_jitter.plugin")
# execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}"/Library/Application Support/vblit/plugins/a_jitter.plugin")
add_custom_command(TARGET a_jitter PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $ENV{HOME}/Library/Application\ Support/wash/plugins/a_jitter.plugin)

if(${OperatingSystem} MATCHES "macos")

add_custom_command(
				TARGET a_jitter POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy
								${CMAKE_CURRENT_BINARY_DIR}/liba_jitter.dylib
		$ENV{HOME}/Library/Application\ Support/wash/brushes/a_jitter.plugin/liba_jitter.plugin.dylib)


#add_custom_command(
#        TARGET a_jitter POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_CURRENT_BINARY_DIR}/liba_jitter.dylib
#		$ENV{HOME}/Library/Application\ Support/comms/plugins/a_jitter.plugin/liba_jitter.plugin.dylib)

endif(${OperatingSystem} MATCHES "macos")
