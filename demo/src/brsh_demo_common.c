//
//  brsh_demo_common.c
//  brsh_demo
//
//  Created by vs on 6/14/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "brsh_demo_common.h"


//BrshDemo* demos[NUM_DEMOS];

int  current_demo_index = 0;

//static double dpi	    = 1;
double display_radius = 1;
bool   faking_it      = false;

//WLineHnd work_line;
//WObjectHnd test_geometry;
BrshPlugin* simple_brush = NULL;

BBrush** brushes = NULL;
int num_brushes = 0;

int window_w = 0;
int window_h = 0;
int frame_w = 0;
int frame_h = 0;
double dpi = 1;

int mouse_down = false;
int mouse_present = false;
double mouse_x = 0;
double mouse_y = 0;
WDocumentHnd document;
//#include "demos/svg.c"

extern BrshDemo brushDemo;

//demos = {brushDemo};
BrshDemo* current_demo = NULL;
BrshDemo* demos[NUM_DEMOS] = {&brushDemo};

bool wash_demo_has_pressure_sensitivity_of_any_kind = false;
//double mouse_present = false;

//bool wash_demo_has_pressure_sensitivity_of_any_kind = false;

void update_brushes(void)
{
	
	
	if (!simple_brush )
	{
		printf("AAACK\n");
		return;
	}
	WDocument* doc = document.src;
	WSequence* seq = doc->sequence.src;
	WObject* frame = seq->frames[0];
	
	
	
	printf("About to update brushes for %d lines.\n",frame->num_lines);
	int num = frame->num_lines;
	if ( brushes )
	{
		free(brushes);
		num_brushes = 0;
	}
	for ( int i =0; i < num; i++)
	{
		WLine* line = frame->lines[i];
		
		num_brushes++;
		
		if ( brushes == NULL)
		{
			brushes = calloc(1, sizeof(BBrush));
			
		}else
		{
			brushes = realloc(brushes, num_brushes* sizeof(BBrush));
		}
		WLineHnd hnd;
		hnd.src = line;
		
		BBrush* b = brsh_brush_create(hnd, 16);
		simple_brush->update(b);
		brushes[num_brushes-1] = b;
	}
	
	
}

int brsh_demo_load_wash(const char* path)
{
	
	document.src = wsh_serial_document_unserialize(path);
	
	//todo: fix this somehow.
	//hackily update the path here so our exports will work
	document.src->meta.path = path;
	
	if (!document.src)
	{
		return 7;
	}
	
	return 0;
	
}


