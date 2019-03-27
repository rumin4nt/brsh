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

//#include <r4/src/core/r_math.h>

//#include <r4/src/geo/r_gpc.h>
#include <wsh/src/ext/wsh_gpc.h>
#include <wsh/src/util/wsh_line_ops.h>
#include <drw/src/drw_config.h>
#include <coer/src/c_platform.h>

static inline double deg2rad(double input)
{
	return M_PI * input / 180.;
}

static inline double angle_from_points(double x1, double y1, double x2, double y2)
{
	return atan2(y2 - y1, x2 - x1);
}

void brsh_brush_update_custom(BBrush* brush, brush_update_func cb);
void brsh_brush_update_new_slow(BBrush* brush);
void brsh_brush_update_old_fast(BBrush* brush);
void brsh_brush_update_tristrip(BBrush* brush);

BBrush* brsh_brush_create(void* data, double width)
{
	WLineHnd* hnd = (WLineHnd*)data;
	//printf("Creating brush with width %f\n", width);
	BBrush* brush       = calloc(1, sizeof(BBrush));
	brush->hnd	  = hnd;
	brush->needs_update = true;
	brush->data	 = NULL;
	brush->data	 = 0;
	brush->stroke       = NULL;
	brush->width	= width;
	brush->update_func  = NULL;
	brush->tess	 = NULL;
	return brush;
}

struct BBrush* brsh_brush_copy(void* wlinehnddata_old, void* wlinehnd)
{
	/*WLineHnd* old = (WLineHnd*)wlinehnddata_old;
	WLineHnd* hnd = (WLineHnd*)wlinehnd;

	if (old == NULL)
	{
		printf("tried to copy a null brush\n");
		return NULL;
	}

	// BBrush* old = src->brush;
	if (!old)
		return NULL;

	BBrush* brush       = brsh_brush_create(hnd, old->src->width);
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
	*/
	return NULL;
}

void brsh_brush_destroy(BBrush* brush)
{
	// free(brush->hnd);
	//printf("NOT destroying handles, brushes don't own base data!", brush->hnd->src);
	//	todo: check and clear other attributes later.

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
	if (!brush)
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

		//brsh_brush_update_new_slow( brush);
		//brsh_brush_update_old_fast(brush);
		brsh_brush_update_tristrip(brush);
	}
}

void brsh_brush_offset(struct BBrush* brush)
{
	//wsh_line_offset(brush, <#double x#>, <#double y#>)
}

static void _brush_tangents(BBrush* brush, WPoint a, WPoint b, double* lx,
			    double* ly, double* rx, double* ry)
{

	double ang = deg2rad(angle_from_points(a.x, a.y, b.x, b.y));
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
	WLine* src		  = brush->hnd->src;

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
//		stroke->has_fill   = true;
//		stroke->has_stroke = true;
//		stroke->fill.r     = 1;
//		stroke->fill.g     = 0;
//		stroke->fill.b     = 1;
//		stroke->fill.a     = .5;

		if (brush->stroke)
		{
			wsh_line_destroy(brush->stroke);
		}

		brush->stroke = stroke;
		printf("Created stroke with %llu points\n", stroke->num);
	}

	brush->needs_update = false;
}

#include <wsh/src/util/wsh_math.h>

void brsh_brush_update_yellowtail(BBrush* brush)
{
	if ( !brush)
	{
		printf("WHOOOOPS\n");
		return;
	}
	WLineHnd* hnd = brush->hnd;
	WLine* base = hnd->src;
	
	
	
	
}


void brsh_brush_update_old_fast(BBrush* brush)
{

	if (!brush)
	{
		printf("OOPS\n");
		return;
	}
	WLineHnd* hnd = brush->hnd;
	WLine* base = hnd->src;
	
	if (!base)
	{
		printf("OOOOPS\n");
		return;
	}
	
	if(!base->data)
	{
		printf("FUUUCK\n");
		return;
	}
	
	if (base->num < 2)
		return;

	WLine* left  = wsh_line_create();
	WLine* right = wsh_line_create();

	//printf("updating brush w %llu points\n", base->num);

	WPoint first = base->data[0];
	wsh_line_add_point(left, first);

	for (unsigned long long i = 1; i < base->num - 1; i++)
	{
		WPoint a = base->data[i - 1];
		WPoint b = base->data[i + 0];
		WPoint c = base->data[i + 1];

		double ps = (a.pressure + b.pressure + c.pressure) / 3;

		double ang = wsh_angle_from_points_wp(a, b);
		ang -= M_PI_2;

		WPoint lp;
		WPoint rp;

		wsh_point_zero(&lp);
		wsh_point_zero(&rp);

		//	old
		//		p2.x = p.x + (ps * cos(ang) * brush->width);

		lp.x = a.x + (cos(ang) * ps * brush->width);
		lp.y = a.y + (sin(ang) * ps * brush->width);
		ang += M_PI;
		rp.x = a.x + (cos(ang) * ps * brush->width);
		rp.y = a.y + (sin(ang) * ps * brush->width);
		wsh_line_add_point(left, lp);
		wsh_line_add_point(right, rp);
	}

	//	last
	wsh_line_add_point(right, base->data[base->num - 1]);

	WLine* stroke = wsh_line_copy(left);

	for (signed long long i = right->num - 1; i > 0; i--)
	{
		wsh_line_add_point(stroke, right->data[i]);
	}

	wsh_line_destroy(left);
	wsh_line_destroy(right);

//	stroke->closed     = true;
//	stroke->has_fill   = true;
//	stroke->has_stroke = true;
//	stroke->fill.r     = 1;
//	stroke->fill.g     = 0;
//	stroke->fill.b     = 1;
//	stroke->fill.a     = .5;

	wsh_line_ops_smooth(stroke, 4);
	//	todo: THIS IS THE ABSOLUTE KEY BIT TO MAKING THE TESSELATED STROKES.
	//	REMOVE IT AT YOUR PERIL;
	//brush->tess = wsh_ext_gpc_tess_create_wline(stroke);
	wsh_line_ops_smooth(stroke, 8);

	if (brush->stroke)
	{
		wsh_line_destroy(brush->stroke);
	}
	brush->stroke       = stroke;
	brush->needs_update = false;
}

void brsh_brush_update_tristrip(BBrush* brush)
{
	//RLine* rl = r_line_create();
	WLine* base = brush->hnd->src;
	
	unsigned long long n = 4 + (base->num * 4 );
	
#ifdef CPLATFORM_IOS
	float* arr = calloc((unsigned)n, sizeof(float));
#else
	double* arr = calloc(n, sizeof(double));
#endif
	
	
	
	for ( unsigned i = 1, j = 0 ; i < base->num - 1 ; i++, j+=4 )
	{
		WPoint a = base->data[i - 1];
		WPoint b = base->data[i + 0];
		WPoint c = base->data[i + 1];
		double ps = (a.pressure + b.pressure + c.pressure) / 3;
		ps = sqrt(ps);
		ps = pow(ps, 2);
		//ps = pow(ps, 2);
		//if ( i == 1 || i == base->num-2 )
		//	ps *= .2;
		
		double ang = wsh_angle_from_points_wp(a, b);
		ang -= M_PI_2;
		
		WPoint lp;
		WPoint rp;
		
		wsh_point_zero(&lp);
		wsh_point_zero(&rp);
		
		lp.x = a.x + (cos(ang) * ps * brush->width);
		lp.y = a.y + (sin(ang) * ps * brush->width);
		ang += M_PI;
		rp.x = a.x + (cos(ang) * ps * brush->width);
		rp.y = a.y + (sin(ang) * ps * brush->width);
		
		arr[j+0] = lp.x;
		arr[j+1] = lp.y;
		arr[j+2] = rp.x;
		arr[j+3] = rp.y;
		
	}
	/*
	WLine* stroke = NULL;
	if ( !brush->stroke )
	{
		stroke = wsh_line_create();
	}else{
		stroke = brush->stroke;
	}
	
	
	wsh_line_ops_smooth(stroke, 4);
	//	todo: THIS IS THE ABSOLUTE KEY BIT TO MAKING THE TESSELATED STROKES.
	//	REMOVE IT AT YOUR PERIL;
	//	idk I think this info ^ might be out of date.
	
	wsh_line_ops_smooth(stroke, 8);
	
	if (brush->stroke)
	{
		wsh_line_destroy(brush->stroke);
	}
	brush->stroke       = stroke;
	*/
	
	brush->needs_update = false;
	
	brush->tristrip = arr;
	//	HACK HACK HACK
	brush->tristripnum = base->num * 2;
	
	//HACK
	brush->tristripnum -= 4;
	
	//	todo: fix thsi  hack
	//drw_tristrip_2df(arr, (base->num * 2) - 4, true);
	//free(arr);
}


