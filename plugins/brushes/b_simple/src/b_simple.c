//
//  v_midi_fighter_twister.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "b_simple.h"

#ifdef DEBUG
#include <stdlib.h>
#endif

//AppSettings   app_settings;
//AppExtensions app_extensions;

static int init(void* plugdata)
{
	BrshPlugin* plug = (BrshPlugin*)plugdata;

	return 0;
}

static int deinit(BrshPlugin* plug)
{
	return 0;
}

static int update(void* plugdata)
{
	return 0;
}

void* brsh_plugin_create(void)
{
	BrshPlugin* plug = calloc(1, sizeof(BrshPlugin));
	plug->type       = BRSH_PLUGIN_TYPE_BRUSH;
	plug->identifier = strdup("space.ruminant.b_simple");
	plug->name       = "TouchOsc";
	plug->update     = update;
	plug->init       = init;
	plug->deinit     = deinit;
	return plug;
}

void brsh_plugin_destroy(void* data)
{

	BrshPlugin* plug = (BrshPlugin*)data;
	free(plug->data);
	free(plug);
#ifdef DEBUG
	printf("Goodbye from plugin land\n");
#endif
}

// AppSettings* app_settings = NULL;
