#include <stdlib.h>
#include <stdio.h>

#include "gl-matrix.h"

//#include <r4/src/r4_platform.h>


void vec3_str(vec3_t vec, char *buffer) {
#ifdef WIN32
	sprintf_s(buffer, sizeof(buffer), "[%f, %f, %f]", vec[0], vec[1], vec[2]);
#else
	sprintf(buffer , "[%f, %f, %f]", vec[0], vec[1], vec[2]);

#endif
	
}

void mat3_str(mat3_t mat, char *buffer)
{
	#ifdef WIN32
    sprintf_s(buffer, sizeof(buffer), "[%f, %f, %f, %f, %f, %f, %f, %f, %f]", mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]);
#else
	    sprintf(buffer, "[%f, %f, %f, %f, %f, %f, %f, %f, %f]", mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]);
#endif
	
}

void mat4_str(mat4_t mat, char *buffer) {
#ifdef WIN32
    sprintf_s(buffer, sizeof(buffer), "[%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f]",
        mat[0], mat[1], mat[2], mat[3], 
        mat[4], mat[5], mat[6], mat[7],
        mat[8], mat[9], mat[10], mat[11], 
        mat[12], mat[13], mat[14], mat[15]);
#else
	sprintf(buffer, "[%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f]",
		  mat[0], mat[1], mat[2], mat[3],
		  mat[4], mat[5], mat[6], mat[7],
		  mat[8], mat[9], mat[10], mat[11],
		  mat[12], mat[13], mat[14], mat[15]);
#endif
}

void quat_str(quat_t quat, char *buffer)
{
#ifdef WIN32
	sprintf_s(buffer, sizeof(buffer), "[%f, %f, %f, %f]", quat[0], quat[1], quat[2], quat[3]);
#else
	sprintf(buffer, "[%f, %f, %f, %f]", quat[0], quat[1], quat[2], quat[3]);

#endif
	
}
