//
//  brsh_demo_common.h
//  brsh_demo
//
//  Created by vs on 6/14/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef brsh_demo_common_h
#define brsh_demo_common_h

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include "demo.h"
#include <stdbool.h>
#include <wsh/wsh.h>

#define NUM_DEMOS 1

extern int window_w;
extern int window_h;
extern int frame_w ;
extern int frame_h;
extern double dpi;
extern double mouse_x;
extern double mouse_y;
extern int mouse_down;
extern int mouse_present;

//static double dpi	    = 1;
extern double display_radius ;
extern bool   faking_it      ;

extern bool wash_demo_has_pressure_sensitivity_of_any_kind;
extern BrshDemo* current_demo;
extern BrshDemo* demos[NUM_DEMOS];
extern int  current_demo_index;

int brsh_demo_load_wash(const char* path);
extern WDocumentHnd document;

#endif /* brsh_demo_common_h */
