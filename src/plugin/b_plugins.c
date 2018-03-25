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

#include <r4/r4.h>
#include <r4/src/io/r_portmidi.h>

static struct vector_t plugins;
extern DebugSettings   debug_settings;

BrshPlugin* brsh_plugin_load(const char* path, const char* name);
// BrshPlugin** plugins = NULL;

void brsh_plugins_load(const char* directory)
{

	int res = vector_init(&plugins, 4, sizeof(BrshPlugin));
	// printf("vector init res: %d\n", res);

	//char* directory = get_path_data();

	char buf[256];
	sprintf(buf, "%s/plugins", directory);

	if (debug_settings.plugin)
		printf("Scanning for plugins in directory: %s\n", buf);
	int successes = 0;

	mkdirp(buf, (mode_t)0755);

	tinydir_dir dir;
	tinydir_open(&dir, buf);
	// char** plugins = NULL;
	int num_plugins = 0;

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (strcmp(file.name, ".") != 0 &&
		    strcmp(file.name, "..") != 0)
		{
			if (wildcardcmp("*.plugin", file.name))
			{
				// if ( plugins == NULL )
				//{

				//}

				// printf("relevant: %s\n", file.name);
				/*
				 if ( plugins == NULL )
				 {
				 plugins = calloc(PATH_MAX, sizeof(char*));
				 }else{

				 plugins = realloc(plugins, PATH_MAX *
				 sizeof(char) * num_plugins );
				 }
				 */
				// plugins[num_plugins] = file.path;
				BrshPlugin* res =
				    brsh_plugin_load(file.path, file.name);
				if (res)
				{
					if (debug_settings.plugin)
						printf(
						    "Plugin load succeeded!\n");
					// BrshPlugin* plug = calloc(1,
					// sizeof(BrshPlugin));
					vector_push_back(&plugins, res);
					successes++;
				}
				else
				{
					if (debug_settings.plugin)
						printf("Plugin load failed!\n");
				}
			}
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	free(directory);
	if (debug_settings.plugin)
		printf("Succesfully loaded %d plugins.\n", successes);
	for (int i = 0; i < plugins.length; i++)
	{
		// printf("%s\n", p)
	}
}

void brsh_plugin_unload(BrshPlugin* plug);

void brsh_plugins_unload(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		BrshPlugin* plug = calloc(1, sizeof(void*));
		vector_get(&plugins, i, plug);
		brsh_plugin_unload(plug);
		// free(plug);
	}
}

void brsh_plugins_init(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		BrshPlugin* plug = calloc(1, sizeof(BrshPlugin));
		vector_get(&plugins, i, plug);
		if (plug->init)
		{
			plug->init(plug);
		}
		free(plug);
	}
}

void brsh_plugins_deinit(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		BrshPlugin* plug = calloc(1, sizeof(BrshPlugin));
		vector_get(&plugins, i, plug);
		if (plug->deinit)
		{
			plug->deinit(plug);
		}
		free(plug);
	}
}

BrshPlugin* brsh_plugin_load(const char* path, const char* name)
{
	if (debug_settings.plugin)
		printf("Attempting to load plugin:\n %s\n", path);

	char buf[PATH_MAX];

	sprintf(buf, "%s%slib%s.%s", path, R4_PATH_SEP, name, R4_SHAREDLIB_EXT);

	if (debug_settings.plugin)
		printf("Looking for dll: %s\n", buf);

	void* handle;
	void* (*vblit_plugin_create)(void);
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

	*(void**)(&vblit_plugin_create) = dlsym(handle, "vblit_plugin_create");
	// fprintf(stderr, "%s\n", dlerror());

	void* foo = vblit_plugin_create();

	BrshPlugin* plug = (BrshPlugin*)foo;
	if (!plug)
	{
		return NULL;
	};

	if (debug_settings.plugin)
		printf("[PLUGIN] : %s %s\n", plug->version, plug->identifier);

	plug->path     = strdup(path);
	plug->filename = strdup(name);

	/*
#ifdef R4_ENABLE_PORTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI) {
		plug->callback = r_portmidi_receive;
	}
#endif

#ifdef R4_ENABLE_RTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI) {
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

	void* (*vblit_plugin_destroy)(void*);
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

	*(void**)(&vblit_plugin_destroy) =
	    dlsym(handle, "vblit_plugin_destroy");

	fprintf(stderr, "%s\n", dlerror());

	if (debug_settings.plugin)
		printf("Destroying plugin: %s\n", plug->filename);
	vblit_plugin_destroy(plug);

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
	if (!plug->update)
	{
		printf("No update method set!\n");
		return;
	}
	plug->update(plug);
}

void brsh_plugins_update(void)
{
	BrshPlugin* plug = calloc(1, sizeof(BrshPlugin));
	for (int i = 0; i < plugins.length; i++)
	{
		//	this is cruft from vblit plugin, need diff workflow.
		///vector_get(&plugins, i, plug);

		//if (plug->enabled && plug->active) {
		//	brsh_plugin_update(plug);
		//}
	}
	free(plug);
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
