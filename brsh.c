

#include "brsh.h"

#include <stdio.h>

int brsh_check_version_match(const char* header_generated_version)
{
	char* v = brsh_get_version_string();
	int match = (0 == strcmp(v, header_generated_version));
	free(v);
	return match;
}

char* brsh_get_version_string(void)
{
	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d_%d_%d", BRSH_VERSION_MAJOR, BRSH_VERSION_MINOR,
		BRSH_VERSION_PATCH);
	return buf;
}

int brsh_init(void)
{
	return 0;
}

int brsh_deinit(void)
{
	return 0;
}
