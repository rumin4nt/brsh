//
//  brsh_plugin.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "brsh_plugins.h"

#include "../../brsh_config.h"

#include "include/tinydir.h"
#include <dlfcn.h>
#include <mkdirp/mkdirp.h>
#include <wildcardcmp/wildcardcmp.h>

#include <stdio.h>
#include <vector/vector.h>

static struct vector_t plugins;

static const char* search_dir = NULL;

//BrshPlugin* brsh_plugin_load(const char* path, const char* name);
// BrshPlugin** plugins = NULL;
void brsh_plugins_set_search(const char* path)
{
	printf("setting search dir to : %s\n", path);
	search_dir = strdup(path);
}

static const char* get_filename_trunk(const char* path)
{
	//from https://stackoverflow.com/questions/43163677/how-do-i-strip-a-file-extension-from-a-string-in-c

	char* fname = strdup(path);
	char* end   = fname + strlen(fname);

	while (end > fname && *end != '.')
	{
		--end;
	}

	if (end > fname)
	{
		*end = '\0';
	}

	return fname;
}

void brsh_plugins_load(const char* directory)
{
}

//void brsh_plugin_unload(BrshPlugin* plug);

void brsh_plugins_unload(void)
{
}

void brsh_plugins_init(void)
{
#ifndef BPLATFORM_IOS
	printf("Loading brushes.\n");
	tinydir_dir dir;
	tinydir_open(&dir, search_dir);

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (file.is_dir)
		{
			char* search = strstr(file.name, ".plugin");
			if (search)
			{
				printf("%s\n", file.name);

				char*       buf   = calloc(PATH_MAX, sizeof(char));
				const char* trunk = get_filename_trunk(file.name);

				sprintf(buf, "%s%s%s", search_dir, B_PATH_SEP, file.name);

				brsh_plugin_load(buf, trunk);
			}
		}
		/*
		printf("%s", file.name);
		if (file.is_dir)
		{
			printf("/");
		}
		printf("\n");
		 */
		tinydir_next(&dir);
	}

	tinydir_close(&dir);
#endif
}

void brsh_plugins_deinit(void)
{
}
//void brsh_plugins_update(BrshPlugin* plug, WLine* line)
////{
//}

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

//#endif
