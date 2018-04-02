
#include "brsh_plugin.h"
#include "../../brsh_config.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

BrshPlugin* brsh_plugin_load(const char* path, const char* name)
{
#ifndef BPLATFORM_IOS
	printf("Attempting to load plugin: %s %s\n", path, name);

	char buf[PATH_MAX];

	sprintf(buf, "%s%slib%s.%s", path, B_PATH_SEP, name, B_SHAREDLIB_EXT);

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
#else
	return NULL;
#endif
}

void brsh_plugin_unload(BrshPlugin* plug)
{
#ifndef BPATFORM_IOS
	printf("Unloading all plugins.\n");

	printf("Attempting to unload plugin:\n %s\n", plug->path);
	char buf[PATH_MAX];

	sprintf(buf, "%s%slib%s.%s", plug->path, B_PATH_SEP, plug->filename, B_SHAREDLIB_EXT);
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
#endif
}
