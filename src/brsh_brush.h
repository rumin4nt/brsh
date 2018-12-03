//
//  brsh_brush.h
//  brsh
//
//  Created by vs on 9/20/17.
//  Copyright © 2017 vs. All rights reserved.
//

#ifndef brsh_brush_h
#define brsh_brush_h

#include <brsh/brsh.h>

#ifdef BRSH_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

enum
{
	B_BRUSH_TYPE_NONE,
	B_BRUSH_TYPE_FULL,
	B_BRUSH_TYPE_DISTANCE,
	B_BRUSH_TYPE_RIDIC
};

typedef struct BBrush
{
	int	   brush_type;
	int	   needs_update;
	double	width;
	void*	 hnd;
	void*	 stroke;
	double*       data;
	void*	 extra;
	void*	 tess;
	unsigned long num;
	void*	 update_func;
} BBrush;

typedef struct BBrushHnd
{
	BBrush* src;
} BBrushHnd;

typedef void (*brush_update_func)(BBrush*);

void brsh_brush_update_custom(BBrush* brush, brush_update_func func);

struct BBrush* brsh_brush_create(void* wlinehnd, double width);
struct BBrush* brsh_brush_copy(struct BBrush* brush, void* wlinehnd);

void brsh_brush_offset(struct BBrush* brush);

void brsh_brush_destroy(struct BBrush*);
void brsh_brush_update(BBrush*);
void brsh_brush_draw(BBrush*);

#endif /* brsh_brush_h */
