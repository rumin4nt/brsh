//
//  brsh_plugin.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "brsh_plugins.h"

#include "../../brsh_config.h"
#include "brsh_plugin.h"

#include "include/tinydir.h"
#include <dlfcn.h>
#include <mkdirp/mkdirp.h>
#include <wildcardcmp/wildcardcmp.h>

#include <stdio.h>

//#include <vector/vector.h>
//static struct vector_t plugins;

static const char* search_dir = NULL;

//BrshPlugin* brsh_plugin_load(const char* path, const char* name);
BrshPlugin** plugins = NULL;
int plugin_num = 0;

void brsh_plugins_set_search(const char* path)
{
	printf("setting search dir to : %s\n", path);
	search_dir = strdup(path);
}

static char* get_filename_trunk(const char* path)
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
	
	
	if ( !search_dir )
	{
		printf("Can't query brushes, no search_dir specified!\n");
		return;
	}
	printf("Loading brushes in dir: [%s].\n", search_dir);
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
				 char* trunk = get_filename_trunk(file.name);

				sprintf(buf, "%s%s%s", search_dir, B_PATH_SEP, file.name);

				BrshPlugin* plug = brsh_plugin_load(buf, trunk);
				if ( plug )
				{
					plugin_num++;
					if ( plugins == NULL )
					{
						plugins = calloc(1, sizeof(BrshPlugin*));
					}else{
						plugins = realloc(plugins, sizeof(BrshPlugin*) * plugin_num);
						
					}
				}
				plugins[plugin_num-1] = plug;
				free(trunk);
				
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
#include <brsh/brsh.h>


void brsh_plugins_update(void)
{
}

BrshPlugin** brsh_plugins_get(int* num)
{
	*num = plugin_num;
	return plugins;
}
/*
void 	brsh_plugins_enumerate(char*** ss, int* num)
{
	printf("Enumerating brushes...\n");
	*num = plugin_num;
	*ss = calloc(BRSH_IDENTIFIER_MAX_LENGTH * plugin_num, sizeof(char));

	for ( int i = 0; i < plugin_num;i ++ )
	{
		//ss[i] = calloc(BRSH_IDENTIFIER_MAX_LENGTH, sizeof(char)); // = calloc(BRSH_IDENTIFIER_MAX_LENGTH * plugin_num, sizeof(char));
		
		*ss[i] = strdup(plugins[i]->identifier);
		printf("%s\n", *ss[i]);
	}
}
*/
BrshPlugin* brsh_plugins_query(const char* identifier)
{
	
	for(int i =0;i < plugin_num; i++ )
	{
		BrshPlugin* p = plugins[i];
		if ( 0 == strcmp(identifier, p->identifier))
			return p;
	}
	return NULL;
}

BrshPlugin* brsh_plugins_instance(const char* identifier)
{
	return NULL;
}

//#endif
