//
//  brsh_plugin.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "b_plugins.h"

#ifdef R4_ENABLE_B_PLUGIN

#include "include/tinydir.h"
#include <dlfcn.h>
#include <mkdirp/mkdirp.h>
#include <wildcardcmp/wildcardcmp.h>

#include <vector/vector.h>

static struct vector_t plugins;

BrshPlugin* brsh_plugin_load(const char* path, const char* name);
// BrshPlugin** plugins = NULL;

void brsh_plugins_load(const char* directory)
{
}

void brsh_plugin_unload(BrshPlugin* plug);

void brsh_plugins_unload(void)
{
}

void brsh_plugins_init(void)
{
}

void brsh_plugins_deinit(void)
{
}

BrshPlugin* brsh_plugin_load(const char* path, const char* name)
{
	return NULL;
}

void brsh_plugin_unload(BrshPlugin* plug)
{
}

void brsh_plugin_update(BrshPlugin* plug, WLine* line)
{
}

void brsh_plugins_update(void)
{
}

int brsh_plugins_query(const char* identifier)
{
	return -1;
}

BrshPlugin* brsh_plugins_instance(const char* identifier)
{
	return NULL;
}

#endif
