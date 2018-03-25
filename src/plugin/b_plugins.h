//
//  brsh_plugin.h
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef brsh_plugin_h
#define brsh_plugin_h

#include <r4/src/r4_config.h>
#include <wsh/src/geo/w_line.h>

#ifdef R4_ENABLE_B_PLUGIN

#include <r4/src/io/r_portmidi.h>

typedef void* (*b_plugin_create)(void);
typedef void (*b_plugin_destroy)(void*);
typedef int (*b_plugin_update)(void*);
typedef int (*b_plugin_init)(void*);
typedef int (*b_plugin_deinit)(void*);

typedef struct
{
	int type;

	b_plugin_create  create;
	b_plugin_destroy destroy;
	b_plugin_update  update;
	b_plugin_init    init;
	b_plugin_deinit  deinit;

	int enabled;
	int active;

	midi_func callback;
	void*     data;

	char* version;
	char* identifier;
	char* filename;
	char* name;
	char* path;
} BrshPlugin;

void brsh_plugins_load(const char* path);
void brsh_plugins_unload(void);

void brsh_plugins_init(void);
void brsh_plugins_deinit(void);

int brsh_plugins_query(const char* identifier);

void	brsh_plugins_update(void);
BrshPlugin* brsh_plugins_instance(const char* identifier);

void brsh_plugin_update(BrshPlugin*, WLine* line);

#endif // RPLATFORM_IOS

#endif /* brsh_plugin_h */
