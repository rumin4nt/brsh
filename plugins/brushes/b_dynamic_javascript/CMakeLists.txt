cmake_minimum_required (VERSION 3.0)

project(b_dynamic_js)

set (b_dynamic_js_VERSION_MAJOR 0)
set (b_dynamic_js_VERSION_MINOR 0)
set (b_dynamic_js_VERSION_PATCH 1)

set(project_name b_dynamic_js)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	 # Mac OS X specific code
	 SET(OperatingSystem "macos")
	 SET(lib_extension dylib)
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Linux specific code
	SET(OperatingSystem "linux")
	SET(lib_extension so)
endif()

IF(WIN32)
	SET(OperatingSystem "windows")
	SET(lib_extension dll)
ENDIF()


if(${OperatingSystem} MATCHES "linux")
	link_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/x86_64)
endif(${OperatingSystem} MATCHES "linux")


FIND_LIBRARY(PM_LIBRARY portaudio)

if(${OperatingSystem} MATCHES "macos")
	LINK_DIRECTORIES(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/contrib/platform/darwin )
endif(${OperatingSystem} MATCHES "macos")

add_library(b_dynamic_js SHARED ${sources})

target_include_directories (b_dynamic_js PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib/include)
target_include_directories (b_dynamic_js PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib)
target_include_directories (b_dynamic_js PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libs)

target_link_libraries(b_dynamic_js m)


if(${OperatingSystem} MATCHES "macos")
	set(plugin_path $ENV{HOME}/Library/Application\ Support)
endif()

if(${OperatingSystem} MATCHES "linux")
	set(plugin_path $ENV{HOME}/.config/space.ruminant)
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${plugin_path})

set(plugin_name ${project_name}.plugin)
set(plugin_dir ${plugin_path}/wash/plugins/brushes/${plugin_name})
set(lib_dest ${plugin_dir}/${project_name}.${lib_extension}})

file(MAKE_DIRECTORY ${plugin_dir})


add_custom_command(
		TARGET b_dynamic_js POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/lib${project_name}.${lib_extension}
		${lib_dest})

set(project_name b_dynamic_js)

build_plugin()
