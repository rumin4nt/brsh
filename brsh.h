//
//  brsh.h
//  brsh
//
//  Created by vs on 9/20/17.
//  Copyright © 2017 vs. All rights reserved.
//

#ifndef brsh_h
#define brsh_h

#include "src/brsh_brush.h"
#include "src/plugin/brsh_plugin.h"
#include "src/plugin/brsh_plugins.h"

#define BRSH_VERSION_MAJOR 0
#define BRSH_VERSION_MINOR 0
#define BRSH_VERSION_PATCH 1

char* brsh_get_version_string(void);
int   brsh_check_version_match(const char* header_generated_version);

int brsh_reload_plugins(void);

int brsh_init(void);
int brsh_deinit(void);

#endif /* brsh_h */
