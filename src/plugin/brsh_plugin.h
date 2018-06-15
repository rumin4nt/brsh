//
//  brsh_plugin.h
//  brsh
//
//  Created by vs on 3/29/18.
//  Copyright © 2018 vs. All rights reserved.
//

#ifndef brsh_plugin_h
#define brsh_plugin_h

//#include <wsh/src/geo/wsh_line.h>

typedef void* (*b_plugin_create)(void);
typedef void (*b_plugin_destroy)(void*);
typedef int (*b_plugin_update)(void*);
typedef int (*b_plugin_init)(void*);
typedef int (*b_plugin_deinit)(void*);

typedef enum {
	BRSH_PLUGIN_TYPE_BRUSH,
	BRSH_PLUGIN_TYPE_AFFECTOR,
	BRSH_PLUGIN_TYPE_NONE

} BrushPluginTypes;

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

	//midi_func callback;
	void* data;

	char* version;
	char* identifier;
	char* filename;
	char* name;
	char* path;
} BrshPlugin;

BrshPlugin* brsh_plugin_load(const char* path, const char* name);

void brsh_plugin_unload(BrshPlugin* plug);

#endif /* brsh_plugin_h */
