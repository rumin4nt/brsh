# Find WSH 3
#
# WSH_LIBRARIES
# WSH_INCLUDE_DIRS.
# WSH_FOUND

IF(NOT UNIX)
    IF(NOT WSH_ROOT)
        MESSAGE("ERROR: WSH_ROOT must be set!")
    ENDIF(NOT WSH_ROOT)

    FIND_PATH(WSH_INCLUDE_DIRS DOC "Path to WSH include directory."
            NAMES WSH/wsh.h
            PATHS ${WSH_ROOT}/include)

    IF(MSVC15)
        FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                NAMES wsh.lib
                PATHS ${WSH_ROOT}/lib-vc2015)
    ELSEIF(MSVC13)
        FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                NAMES wsh.lib
                PATHS ${WSH_ROOT}/lib-vc2013)
    ELSEIF(MSVC12)
        FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                NAMES wsh.lib
                PATHS ${WSH_ROOT}/lib-vc2012)
    ELSEIF(MSVC10)
        FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                NAMES wsh.lib
                PATHS ${WSH_ROOT}/lib-vc2010)
    ELSEIF(MINGW)
        IF(CMAKE_CL_64)
            FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                    NAMES wsh.dll
                    PATHS ${WSH_ROOT}/lib-mingw-w64)
        ELSE(CMAKE_CL_64)
            FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                    NAMES wsh.dll
                    PATHS ${WSH_ROOT}/lib-mingw)
        ENDIF(CMAKE_CL_64)
    ELSE(MINGW)
        # Default to latest version of VC libs
        FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
                NAMES wsh.lib
                PATHS ${WSH_ROOT}/lib-vc2015)
    ENDIF(MSVC15)
ELSE(NOT UNIX)
    FIND_PATH(WSH_INCLUDE_DIRS DOC "Path to WSH include directory."
            NAMES WSH/wsh.h
            PATHS
            /usr/include
            /usr/local/include
            /usr/target/include
            /sw/include
            /opt/local/include)

    FIND_LIBRARY(WSH_LIBRARIES DOC "Absolute path to WSH library."
            NAMES wsh.dll wsh.dylib libwsh.dylib
            PATHS
            /usr/local/lib
            /usr/lib
            /lib)
ENDIF(NOT UNIX)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WSH DEFAULT_MSG WSH_LIBRARIES WSH_INCLUDE_DIRS)

mark_as_advanced(WSH_INCLUDE_DIRS WSH_LIBRARIES)
