//
//  b_brush.h
//  brsh
//
//  Created by vs on 9/20/17.
//  Copyright © 2017 vs. All rights reserved.
//

#ifndef b_brush_h
#define b_brush_h

//#include <wsh/wsh.h>

#include <stdbool.h>
#include <wsh/src/geo/w_line.h>

enum
{
	B_BRUSH_TYPE_NONE,
	B_BRUSH_TYPE_FULL,
	B_BRUSH_TYPE_DISTANCE,
	B_BRUSH_TYPE_RIDIC
};

typedef struct BBrush
{
	int		brush_type;
	bool		needs_update;
	double		width;
	struct WLineHnd hnd;
	struct WLine*   stroke;
	double*		data;
	void*		extra;
	void*		tess;
	unsigned long   num;
	void*		update_func;
} BBrush;

typedef struct BBrushHnd
{
	BBrush* src;
} BBrushHnd;

typedef void (*brush_update_func)(BBrush*);

void b_brush_update_custom(BBrush* brush, brush_update_func func);

struct BBrush* b_brush_create(struct WLineHnd hnd, double width);
struct BBrush* b_brush_copy(struct BBrush* brush, struct WLineHnd hnd);

void b_brush_offset(struct BBrush* brush);

void b_brush_destroy(struct BBrush*);
void b_brush_update(BBrush*);
void b_brush_draw(BBrush*);

#endif /* b_brush_h */
