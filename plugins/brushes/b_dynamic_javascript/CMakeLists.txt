                                                  cmake_minimum_required (VERSION 3.0)

project(b_dynamic_js)

set (b_dynamic_js_VERSION_MAJOR 0)
set (b_dynamic_js_VERSION_MINOR 0)
set (b_dynamic_js_VERSION_PATCH 1)

set(project_name b_dynamic_js)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c

)

IF(${OperatingSystem} MATCHES "macos")
	# Mac OS X specific code
	SET(lib_extension dylib)
endif()

IF(${OperatingSystem} MATCHES "linux")
	# Linux specific code
	SET(lib_extension so)

endif()

IF({OperatingSystem} MATCHES "windows")
	SET(lib_extension dll)
ENDIF()



add_library(${project_name} SHARED ${sources})

target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib/include)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../contrib)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../libs)

target_link_libraries(${project_name} m)




if(${OperatingSystem} MATCHES "macos")
	set(plugin_path $ENV{HOME}/Library/Application\ Support)
endif()

if(${OperatingSystem} MATCHES "linux")
	set(plugin_path $ENV{HOME}/.config/space.ruminant)
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${plugin_path})

set(lib_name "lib${project_name}.${lib_extension}")
set(plugin_name ${project_name}.plugin)
set(plugin_dir ${plugin_path}/wash/plugins/${plugin_name})
set(lib_dest ${plugin_dir}/lib${project_name}.${lib_extension}})


file(MAKE_DIRECTORY ${plugin_dir})

add_custom_command(
		TARGET ${project_name} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/lib${project_name}.${lib_extension}
		${lib_dest})
