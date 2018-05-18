cmake_minimum_required(VERSION 3.0)
project(test_brsh)
set (test_brsh_VERSION_MAJOR 0)
set (test_brsh_VERSION_MINOR 0)
set (test_brsh_VERSION_PATCH 1)

file(GLOB sources *.c)
message(${sources})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/platform/darwin)

add_executable(test_brsh ${sources})
target_include_directories (test_brsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs)
target_include_directories (test_brsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/include)
target_include_directories (test_brsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib)
target_link_libraries(test_brsh LINK_PUBLIC brsh)

# target_compile_options(test_brsh PUBLIC )
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  	 SET(OperatingSystem "macos")
	set (CMAKE_C_FLAGS "-pagezero_size 10000 -image_base 100000000")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#		conf.env.append_unique("LINKFLAGS", ["-mmacosx-version-min=10.9", '-pagezero_size 10000', '-image_base 100000000', '-w'])


