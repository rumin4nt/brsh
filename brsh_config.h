//
//  brsh_config.h
//  brsh
//
//  Created by vs on 3/29/18.
//  Copyright © 2018 vs. All rights reserved.
//

#ifndef brsh_config_h
#define brsh_config_h

#ifdef _WIN64
#define BPLATFORM_WIN
#elif _WIN32
#define BPLATFORM_WIN

#elif __APPLE__
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#define BPLATFORM_IOS
//#define BPLATFORM_IOS
#elif TARGET_IPHONE_SIMULATOR
//#define BPLATFORM_IOS_SIM
//#define BPLATFORM_IOS
#define BPLATFORM_IOS
#define PATH_MAX 2048

#else

#define BPLATFORM_DARWIN 1
#undef BPLATFORM_IOS_SIM
#undef BPLATFORM_IOS
#endif

#elif __linux
#define BPLATFORM_LINUX

#elif __linux__
#define BPLATFORM_LINUX
#elif __unix // all unices not caught above

#define BPLATFORM_UNIX
#error unix
#elif __posix

#define BPLATFORM_POSIX
#error posix
#else

#error unable to determine platform
#endif

#ifdef BPLATFORM_LINUX
#include <linux/limits.h>
#include <stdbool.h>
#define B_PATH_SEP "/"
#define B_SHAREDLIB_EXT "so"
#endif

#ifdef BPLATFORM_WIN

#include <windows.h>
#define PATH_MAX 2048
typedef short bool;
#define true 1
#define false 0

#define B_PATH_SEP "\\"
#define B_SHAREDLIB_EXT "dll"
#endif

#ifdef BPLATFORM_DARWIN
#include <limits.h>
#include <stdbool.h>

#define B_PATH_SEP "/"
#define B_SHAREDLIB_EXT "dylib"
#endif

#ifdef BPLATFORM_IOS
#define PATH_MAX 4096
#define B_PATH_SEP "/"
#define B_SHAREDLIB_EXT "dll"
#endif

#endif /* brsh_config_h */
