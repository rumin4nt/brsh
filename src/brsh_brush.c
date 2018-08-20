//
//  brsh_brush.c
//  brsh
//
//  Created by vs on 9/20/17.
//  Copyright © 2017 vs. All rights reserved.
//

#include "brsh_brush.h"

#include <stdbool.h>
#include <stdio.h>

#include <math.h>
#include <string.h>

#include <brsh/brsh.h>

#include <r4/src/core/r_math.h>
#include <r4/src/geo/r_gpc.h>
#include <wsh/src/util/wsh_line_ops.h>

void brsh_brush_update_custom(BBrush* brush, brush_update_func cb);
void brsh_brush_update_new_slow(BBrush* brush);
void brsh_brush_update_old_fast(BBrush* brush);

BBrush* brsh_brush_create(struct WLineHnd ref, double width)
{
	//printf("Creating brush with width %f\n", width);
	BBrush* brush       = calloc(1, sizeof(BBrush));
	brush->hnd	  = ref;
	brush->needs_update = true;
	brush->data	 = NULL;
	brush->data	 = 0;
	brush->stroke       = NULL;
	brush->width	= width;
	brush->update_func  = NULL;
	brush->tess	 = NULL;
	return brush;
}

BBrush* brsh_brush_copy(struct BBrush* old, struct WLineHnd hnd)
{
	
	if (old == NULL)
	{
		printf("tried to copy a null brush\n");
		return NULL;
	}
	
	// BBrush* old = src->brush;
	if (!old)
		return NULL;
	
	BBrush* brush       = brsh_brush_create(hnd, old->width);
	brush->needs_update = old->needs_update;
	
	if (old->stroke)
	{
		// brush->stroke = calloc(1, sizeof(WLine));
		brush->stroke = wsh_line_copy(old->stroke);
		// memcpy(brush->stroke, old->stroke, sizeof(WLine));
	}
	
	brush->hnd = hnd;
	printf("Copied a brush for line %p\n", old);
#ifdef DISABLE_UNTIL_WORKLINE_REFACTOR_COMPLETE
	if (old->stroke)
	{
		if (old->stroke->tess)
		{
			w_gpc_tess_create(brush->stroke);
		}
	}
#endif
	return brush;
}

void brsh_brush_destroy(BBrush* brush)
{
	// free(brush->hnd);
	brush->hnd.src = NULL;
	free(brush->data);
	// printf("Destroying a brush for line %p\n", brush->hnd);
	// free(brush);
}

/*
 static inline float angle_from_points(int x1, int y1, int x2, int y2)
 {
 float dx = x2 - x1;
 float dy = y2 - y1;
 float angle = atan2(dy,dx);
 //return atan2(dy, dx);
 
 
 
 if (dy < 0 ) {
 angle += 2 * (float)M_PI;
 }
 // map to [0, 1] range
 angle /= (2 * (float)M_PI);
 angle *= 360;
 angle -= 270;
 return angle;
 }
 
 
 static inline float angle_from_points_wp(WPoint a, WPoint b)
 {
 return angle_from_points(a.x,a.y, b.x,b.y);
 }
 
 */

/*
 
 static inline float deg2rad(float input){
 
 return M_PI * input / 180 ;
 }
 
 */

//#define mult .125

void brsh_brush_update_custom(BBrush* brush, brush_update_func func)
{
	func(brush);
}

void brsh_brush_update(BBrush* brush, brush_update_func func)
{
	if ( !brush )
	{
		printf("Passed a bad value to %s", __FUNCTION__);
		return;
	}
	//if ( brush->hnd.src )
	
	
	//	if we pass a function directly to this function, can assume we are overriding.
	if (func)
	{
		
		func(brush);
		return;
	}
	
	if (brush->update_func)
	{
		brush_update_func f = (brush_update_func)brush->update_func;
		f(brush);
	}
	else
	{
		
		//f(brush);
		
		// brsh_brush_update_new_slow( brush);
		brsh_brush_update_old_fast(brush);
	}
}

void brsh_brush_offset(struct BBrush* brush)
{
	//wsh_line_offset(brush, <#double x#>, <#double y#>)
}

static void _brush_tangents(BBrush* brush, WPoint a, WPoint b, double* lx,
			    double* ly, double* rx, double* ry)
{
	
	double ang = deg2rad(angle_from_points_wp(a, b));
	double ps  = a.pressure;
	
	*lx = a.x - (ps * cos(ang) * brush->width);
	*ly = a.y - (ps * sin(ang) * brush->width);
	
	*rx = b.x + (ps * cos(ang) * brush->width);
	*ry = b.y + (ps * sin(ang) * brush->width);
}

void brsh_brush_update_new_slow(BBrush* brush)
{
	if (!brush)
	{
		printf("oops");
		//		oops
		return;
	}
	
	// printf("Updating brush!\n");
	
	double nx, ny;
	double px, py;
	double x, y;
	px = py = x = y = nx = ny = 0;
	WLine* src		  = brush->hnd.src;
	
	WLine* cpy = wsh_line_copy(src);
	wsh_line_ops_smooth(cpy, 4);
	printf("cpy line for brush has %llu points\n", cpy->num);
	printf("src line for brush has %llu points\n", src->num);
	WLine* stroke = wsh_line_create();
	for (int i = 0; i < cpy->num - 1; i++)
	{
		
		WPoint p = cpy->data[i];
		
		x = p.x;
		y = p.y;
		
		WPoint np = cpy->data[i + 1];
		
		nx = np.x;
		ny = np.y;
		
		//	first
		if (i == 0)
		{
			wsh_line_add_point(stroke, p);
		}
		
		if (i > 0 && i < cpy->num - 1)
		{
			
			double lx;
			double ly;
			double rx, ry;
			
			_brush_tangents(brush, p, np, &lx, &ly, &rx, &ry);
			
			WPoint* l = wsh_point_create_2f(lx, ly);
			WPoint* r = wsh_point_create_2f(rx, ry);
			wsh_line_add_point(stroke, *l);
			wsh_line_add_point(stroke, *r);
		}
		
		if (i == cpy->num - 2)
		{
			wsh_line_add_point(stroke, p);
		}
		
		stroke->closed     = true;
		stroke->has_fill   = true;
		stroke->has_stroke = true;
		stroke->fill.r     = 1;
		stroke->fill.g     = 0;
		stroke->fill.b     = 1;
		stroke->fill.a     = .5;
		
		if (brush->stroke)
		{
			wsh_line_destroy(brush->stroke);
		}
		
		brush->stroke = stroke;
		printf("Created stroke with %llu points\n", stroke->num);
	}
	
	brush->needs_update = false;
}

void brsh_brush_update_old_fast(BBrush* brush)
{
	
	
	if (!brush)
	{
		printf("oops");
		//		oops
		return;
	}
	
	// printf("Updating brush!\n");
	
	//	todo make this not horrible
	WLine* left  = wsh_line_create();
	WLine* right = wsh_line_create();
	
	WLine* l = brush->hnd.src;
	if (!l)
		return;
	
	if (l->num < 2)
		return;
	unsigned long long num = l->num;
	
	//WPoint first = l->data[0];
	//wsh_line_add_point(left, first);
	
	for (int i = 1; i < num; ++i)
	{
		WPoint p  = l->data[i];
		double ps = p.pressure;
		
		double ang = 0;
		if (i > 1)
		{
			WPoint before = l->data[i - 1];
			double d      = deg2rad(angle_from_points_wp_deg(p, before));
			ang += d;
		}
		
		WPoint p1, p2;
		
		wsh_point_zero(&p1);
		wsh_point_zero(&p2);
		
		p1.x = p.x - (ps * cos(ang) * brush->width);
		p1.y = p.y - (ps * sin(ang) * brush->width);
		
		p2.x = p.x + (ps * cos(ang) * brush->width);
		p2.y = p.y + (ps * sin(ang) * brush->width);
		
		wsh_line_add_point(left, p1);
		wsh_line_add_point(right, p2);
	}
	
	WLine* stroke = wsh_line_copy(left);
	//	todo, replace this loop with the version that I've surely
	//	already added to the class, yes
	
	//wsh_line_concat(stroke, right, -1, -1);
	
	for (signed long long i = right->num - 1; i > 0; i--)
	{
		wsh_line_add_point(stroke, right->data[i]);
	}
	
	// drw_color(0,0,0,.5);
	
	stroke->closed     = true;
	stroke->has_fill   = true;
	stroke->has_stroke = true;
	stroke->fill.r     = 1;
	stroke->fill.g     = 0;
	stroke->fill.b     = 1;
	stroke->fill.a     = .5;
	
	//	IMPORTANT
	wsh_line_ops_smooth(stroke, 4);
	
	brush->tess = r_gpc_tess_create_wline(stroke);
	wsh_line_ops_smooth(stroke, 8);
	
	// drw_poly(stroke);
	
	wsh_line_destroy(left);
	wsh_line_destroy(right);
	
	if (brush->stroke)
	{
		wsh_line_destroy(brush->stroke);
	}
	brush->stroke = stroke;
	
	brush->needs_update = false;
}
