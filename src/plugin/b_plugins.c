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

static const char* search_dir = NULL;

BrshPlugin* brsh_plugin_load(const char* path, const char* name);
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

void brsh_plugin_unload(BrshPlugin* plug);

void brsh_plugins_unload(void)
{
}

void brsh_plugins_init(void)
{
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
			if ( search)
			{
				printf("%s\n", file.name);
				
				char* buf = calloc(PATH_MAX, sizeof(char));
				const char* trunk =                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       get_filename_trunk(file.name);
				
				sprintf(buf, "%s%s%s", search_dir, R4_PATH_SEP, file.name);
				
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
	
}

void brsh_plugins_deinit(void)
{
}

BrshPlugin* brsh_plugin_load(const char* path, const char* name)
{
	printf("Attempting to load plugin: %s %s\n", path, name);
	
	char buf[PATH_MAX];
	
	sprintf(buf, "%s%slib%s.%s", path, R4_PATH_SEP, name, R4_SHAREDLIB_EXT);
	
	//if (debug_settings.plugin)
		printf("Looking for dll: %s\n", buf);
	
	void* handle;
	void* (*brsh_plugin_create)(void);
	// double (*cosine)(double);
	// void* (*vblit_plugin_destroy)(void*);
	// char* error;
	
	handle = dlopen(buf, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		return NULL;
	}
	
	dlerror(); // Clear any existing error
	
	// Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
	//    would seem more natural, but the C99 standard leaves
	//   casting from "void *" to a function pointer undefined.
	//   The assignment used below is the POSIX.1-2003 (Technical
	//    Corrigendum 1) workaround; see the Rationale for the
	//    POSIX specification of dlsym().
	
	*(void**)(&brsh_plugin_create) = dlsym(handle, "brsh_plugin_create");
	// fprintf(stderr, "%s\n", dlerror());
	
	void* foo = brsh_plugin_create();
	
	BrshPlugin* plug = (BrshPlugin*)foo;
	if (!plug)
	{
		dlclose(handle);
		return NULL;
	};
	
	//if (debug_settings.plugin)
		printf("[PLUGIN] : %s %s\n", plug->version, plug->identifier);
	
	plug->path     = strdup(path);
	plug->filename = strdup(name);
	/*
#ifdef R4_ENABLE_PORTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI)
	{
		plug->callback = r_portmidi_receive;
	}
#endif
	
#ifdef R4_ENABLE_RTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI)
	{
		printf("fixme: NOT setting a  midi callback because I haven't written it yet.\n");
	}
#endif
	*/
	
	plug->active  = true;
	plug->enabled = true;
	
	//	todo: remove this hack
	if (0 == strcmp(plug->identifier,
			"space.ruminant.v_spurious_midi_emitter"))
	{
		printf("Disabling the spurious midi emitter!\n");
		plug->active  = false;
		plug->enabled = false;
	}
	
	return plug;
	
}

void brsh_plugin_unload(BrshPlugin* plug)
{
	printf("Unloading all plugins.\n");
	
	printf("Attempting to unload plugin:\n %s\n", plug->path);
	char buf[PATH_MAX];
	
	sprintf(buf, "%s%slib%s.%s", plug->path, R4_PATH_SEP, plug->filename,
		R4_SHAREDLIB_EXT);
	printf("Looking for dll: %s\n", buf);
	
	void* handle;
	
	void* (*brsh_plugin_destroy)(void*);
	char* error;
	
	handle = dlopen(buf, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		return;
	}
	
	dlerror(); // Clear any existing error
	
	// Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
	//    would seem more natural, but the C99 standard leaves
	//   casting from "void *" to a function pointer undefined.
	//   The assignment used below is the POSIX.1-2003 (Technical
	//    Corrigendum 1) workaround; see the Rationale for the
	//    POSIX specification of dlsym().
	
	*(void**)(&brsh_plugin_destroy) = dlsym(handle, "brsh_plugin_destroy");
	
	fprintf(stderr, "%s\n", dlerror());
	
	//if (debug_settings.plugin)
	printf("Destroying plugin: %s\n", plug->filename);
	brsh_plugin_destroy(plug);
	
	// *(void**)(&cosine) = dlsym(handle, "cos");
	
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);
		return;
	}
	
	dlclose(handle);
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
