//
//  brsh_plugin.h
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef brsh_plugins_h
#define brsh_plugins_h

#include "brsh_plugin.h"

void brsh_plugins_load(const char* path);
void brsh_plugins_unload(void);
void brsh_plugins_set_search(const char* path);

void brsh_plugins_init(void);
void brsh_plugins_deinit(void);

int brsh_plugins_query(const char* identifier);

void	brsh_plugins_update(void);
BrshPlugin* brsh_plugins_instance(const char* identifier);

//void brsh_plugins_update(BrshPlugin* plug, WLine* line);

//#endif // RPLATFORM_IOS

#endif /* brsh_plugins_h */
