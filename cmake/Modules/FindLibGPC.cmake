# Find GPC 3
#
# GPC_LIBRARIES
# GPC_INCLUDE_DIRS.
# GPC_FOUND

IF(NOT UNIX)
    IF(NOT GPC_ROOT)
        MESSAGE("ERROR: GPC_ROOT must be set!")
    ENDIF(NOT GPC_ROOT)

    FIND_PATH(GPC_INCLUDE_DIRS DOC "Path to GPC include directory."
            NAMES GPC/gpc.h
            PATHS ${GPC_ROOT}/include)

    IF(MSVC15)
        FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                NAMES gpc.lib
                PATHS ${GPC_ROOT}/lib-vc2015)
    ELSEIF(MSVC13)
        FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                NAMES gpc.lib
                PATHS ${GPC_ROOT}/lib-vc2013)
    ELSEIF(MSVC12)
        FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                NAMES gpc.lib
                PATHS ${GPC_ROOT}/lib-vc2012)
    ELSEIF(MSVC10)
        FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                NAMES gpc.lib
                PATHS ${GPC_ROOT}/lib-vc2010)
    ELSEIF(MINGW)
        IF(CMAKE_CL_64)
            FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                    NAMES gpc.dll
                    PATHS ${GPC_ROOT}/lib-mingw-w64)
        ELSE(CMAKE_CL_64)
            FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                    NAMES gpc.dll
                    PATHS ${GPC_ROOT}/lib-mingw)
        ENDIF(CMAKE_CL_64)
    ELSE(MINGW)
        # Default to latest version of VC libs
        FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
                NAMES gpc.lib
                PATHS ${GPC_ROOT}/lib-vc2015)
    ENDIF(MSVC15)
ELSE(NOT UNIX)
    FIND_PATH(GPC_INCLUDE_DIRS DOC "Path to GPC include directory."
            NAMES GPC/gpc.h
            PATHS
            /usr/include
            /usr/local/include
            /usr/target/include
            /sw/include
            /opt/local/include)

    FIND_LIBRARY(GPC_LIBRARIES DOC "Absolute path to GPC library."
            NAMES gpc.dll glfw.dylib libglfw.dylib
            PATHS
            /usr/local/lib
            /usr/lib
            /lib)
ENDIF(NOT UNIX)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GPC DEFAULT_MSG GPC_LIBRARIES GPC_INCLUDE_DIRS)

mark_as_advanced(GPC_INCLUDE_DIRS GPC_LIBRARIES)
