//
//  svg.c
//  brsh_demo
//
//  Created by vs on 6/18/18.
//  Copyright © 2018 ruminant. All rights reserved.
//




#ifndef svg_c
#define svg_c

#include "../demo.h"
#include "../brsh_demo_common.h"

#define DEMO_NAME "svg_render"
#define DEMO_NICENAME "SVG Render"

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

static void init(void)
{
	printf("%s init!\n", DEMO_NICENAME);
	
	
	
	if ( !document.src  )
	{
		printf("Nope!\n");
		return;
	}
	
	//const char* dat = wsh_serial_document_serialize(document.src);
	const char* dat = wsh_serial_svg_document_serialize(document.src);
	
	printf("dat:\n%s\n", dat);
	
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
	drop
};

#endif
