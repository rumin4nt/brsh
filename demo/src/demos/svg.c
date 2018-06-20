//
//  svg.c
//  brsh_demo
//
//  Created by vs on 6/18/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef svg_c
#define svg_c

#include "../brsh_demo_common.h"
#include "../demo.h"

#define DEMO_NAME "svg_render"
#define DEMO_NICENAME "SVG Render"

#include <cairo/cairo-svg.h>
#include <cairo/cairo.h>
cairo_surface_t* surface = NULL;

static void tablet_prox(int v)
{
	printf("got tablet prox? %d\n", v);
}

static void tablet_up(double x, double y, int button, double p, double r, double tx, double ty, double altitude, double azimuth, double idk)
{
	printf("got rich up? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

static void tablet_down(double x, double y, int button, double p, double r, double tx, double ty, double altitude, double azimuth, double idk)
{
	printf("got rich down? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

static void tablet_drag(double x, double y, int button, double p, double r, double tx, double ty, double altitude, double azimuth, double idk)
{
	printf("got rich drag? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

static void tablet_motion(double x, double y, int button, double p, double r, double tx, double ty, double altitude, double azimuth, double idk)
{
	printf("got rich motion? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

static void key(int key, int action, int mods)
{
}

static void mouse_move(double x, double y)
{
}

static void mouse_button(int button, int action, int mods)
{
}

#include <wsh/src/serial/wsh_serial_svg.h>

#include <cairo/cairo-svg.h>
#include <cairo/cairo.h>

static int canvas_w = -1;
static int canvas_h = -1;

static void hack_point(WPoint* p)
{
	p->x += canvas_w * .5;
	p->y += canvas_h * .5;
	p->y = canvas_h - p->y;
}







static void w_serialize_line_svg(cairo_t* cr, WLine* line)
{

	cairo_new_path(cr);
	for (int j = 1; j < line->num; j++)
	{
		WPoint prev = line->data[j - 1];
		WPoint p    = line->data[j];
		//prev.x += canvas_w * .5;
		//prev.y += canvas_h * .5;
		hack_point(&p);
		hack_point(&prev);
		cairo_move_to(cr, prev.x, prev.y);
		cairo_line_to(cr, p.x, p.y);
	}

	cairo_status_t status = cairo_surface_status(surface);

	WPoint last = line->data[line->num - 1];
	hack_point(&last);
	cairo_move_to(cr, last.x, last.y);

	printf("Cairo:%s\n", cairo_status_to_string(status));
	
	cairo_close_path(cr);

	cairo_set_source_rgb(cr, 0.5, 0.5, 1);
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, 0.5, 0, 0, 0.5);
	cairo_set_line_width(cr, 10.0);
	cairo_stroke_preserve(cr);

	cairo_set_source_rgb(cr, 0.5, 0.5, 1);
	cairo_fill(cr);

	
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	status =	cairo_surface_status(surface);
	printf("Cairo:%s\n", cairo_status_to_string(status));
	
	cairo_set_source_rgb (cr, 0.5, 0.5, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
	cairo_set_line_width (cr, 10.0);
	
	status =	cairo_surface_status(surface);
	printf("Cairo:%s\n", cairo_status_to_string(status));
	
	cairo_stroke (cr);
	
	
	
	/*
	//cairo_set_source_rgba(cr, 0,1,0,.5);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
	cairo_fill(cr);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
	cairo_close_path(cr);
*/
}

static void serialize_brushes(WDocument* doc)
{
	const char* path
	cairo_t*	 cr;
	WDocumentMeta    meta = doc->meta;
	canvas_w	      = doc->meta.canvas_width;
	canvas_h	      = doc->meta.canvas_height;

	surface = cairo_svg_surface_create(path, canvas_w, canvas_h);
	cr      = cairo_create(surface);

	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_source_rgba(cr, 0, 0, 0, .5);

	for (int i = 0; i < num_brushes; i++)
	{
		BBrush* b = brushes[i];
		WLine*  l = b->stroke;

		if (!l)
		{
			printf("Something went wrong!\n");
			continue;
		}
		w_serialize_line_svg(cr, l);
	}
	/*for ( int i = 0;i < obj->num_lines; i++ )
	{
		WLine* l = obj->lines[i];


	}*/

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}



static void init(void)
{
	printf("%s init!\n", DEMO_NICENAME);

	if (!document.src)
	{
		printf("Nope!\n");
		return;
	}

	//const char* dat = wsh_serial_document_serialize(document.src);

	//wsh_serial_svg_document_serialize(document.src);

	serialize_brushes(document.src);

	//printf("dat:\n%s\n", dat);
}

static void deinit(void)
{
	printf("%s deinit!\n", DEMO_NICENAME);
}

static void update(void)
{
}

static void draw(void)
{
}

static void drop(int num, const char** paths)
{
}

BrshDemo brushDemo =
    {
	DEMO_NICENAME,
	1.0 / 60.0,
	init,
	deinit,
	update,
	draw,
	key,
	mouse_move,
	mouse_button,
	tablet_up,
	tablet_down,
	tablet_motion,
	tablet_drag,
	drop};

#endif
