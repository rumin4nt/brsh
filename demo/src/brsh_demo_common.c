//
//  brsh_demo_common.c
//  brsh_demo
//
//  Created by vs on 6/14/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "brsh_demo_common.h"

BrshDemo* current_demo = NULL;
BrshDemo* demos[NUM_DEMOS];

int  current_demo_index = 0;

//static double dpi	    = 1;
double display_radius = 1;
bool   faking_it      = false;

//WLineHnd work_line;
//WObjectHnd test_geometry;

int window_w = 0;
int window_h = 0;
int frame_w = 0;
int frame_h = 0;
double dpi = 1;

int mouse_down = false;
int mouse_present = false;
double mouse_x = 0;
double mouse_y = 0;
//WDocumentHnd document;
bool wash_demo_has_pressure_sensitivity_of_any_kind = false;
//double mouse_present = false;

//bool wash_demo_has_pressure_sensitivity_of_any_kind = false;
