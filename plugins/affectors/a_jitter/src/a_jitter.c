//
//  v_midi_fighter_twister.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "a_jitter.h"

#include <brsh/src/plugin/brsh_plugin.h>

static int init(void* plugdata)
{
	BrshPlugin* plug = (BrshPlugin*)plugdata;

	return 0;
}

static int deinit(void* plugdata)
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
	plug->type       = BRSH_PLUGIN_TYPE_AFFECTOR;
	plug->identifier = strdup("space.ruminant.v_touchosc");
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
	printf("Goodbye from plugin land\n");
}

// AppSettings* app_settings = NULL;
