//
//  v_midi_fighter_twister.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "b_simple.h"
#include <wsh/src/geo/wsh_line.h>

#include <brsh/src/plugin/brsh_plugin.h>
#include <math.h>
#include <stdio.h>

static inline double angle_from_points(double x1, double y1, double x2, double y2)
{
	double dx    = x2 - x1;
	double dy    = y2 - y1;
	double angle = atan2(dy, dx);
	// return atan2(dy, dx);

	if (dy < 0)
	{
		angle += 2 * (double)M_PI;
	}
	// map to [0, 1] range
	angle /= (2 * (double)M_PI);
	angle *= 360;
	angle -= 270;
	return angle;
}
static inline double angle_from_points_wp(WPoint a, WPoint b)
{
	return angle_from_points(a.x, a.y, b.x, b.y);
}

static inline double deg2rad(float input)
{

	return M_PI * input / 180;
}

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
	BBrush* brush = (BBrush*)plugdata;

	if (!brush)
	{
		printf("oops");
		//		oops
		return 1;
	}

	// printf("Updating brush!\n");

	//	todo make this not horrible
	WLine* left  = wsh_line_create();
	WLine* right = wsh_line_create();

	WLineHnd* hnd = &brush->hnd;
	WLine*    l   = hnd->src;
	if (!l)
		return 2;

	if (l->num < 2)
		return 3;
	unsigned long long num = l->num;

	WPoint first = l->data[0];
	wsh_line_add_point(left, first);

	for (int i = 1; i < num; ++i)
	{
		WPoint p  = l->data[i];
		double ps = p.pressure;

		double ang = 0;
		if (i > 1)
		{
			WPoint before = l->data[i - 1];
			double d      = deg2rad(angle_from_points_wp(p, before));
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

	// stroke->closed     = true;
	// stroke->has_fill   = true;
	// stroke->has_stroke = true;
	// stroke->fill.r     = 1;
	// stroke->fill.g     = 0;
	// stroke->fill.b     = 1;
	// stroke->fill.a     = .5;

	//	IMPORTANT
	//wsh_line_ops_smooth(stroke, 4);

	//brush->tess = r_gpc_tess_create(stroke);
	//wsh_line_ops_smooth(stroke, 8);

	// drw_poly(stroke);

	wsh_line_destroy(left);
	wsh_line_destroy(right);

	if (brush->stroke)
	{
		wsh_line_destroy(brush->stroke);
	}
	brush->stroke = stroke;

	brush->needs_update = false;
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
