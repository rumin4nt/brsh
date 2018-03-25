

#include <stdlib.h>

#include <brsh/brsh.h>

int main(int argc, char** argv)
{

	printf("Running brsh tests.\n");
	printf("making sure drw header matches library.\n");

	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d_%d_%d", BRSH_VERSION_MAJOR, BRSH_VERSION_MINOR,
		BRSH_VERSION_PATCH);

	if (brsh_check_version_match(buf)) {
		printf("Version matches.\n");
	} else {
		printf("Version does not match!!\n");
		free(buf);
		return 88;
	}

	int res = brsh_init();
	printf("brsh_init res: %d\n", res);

	brsh_deinit();
	free(buf);
	return 0;
}
