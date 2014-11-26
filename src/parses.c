#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int parse_int(char *buffer) {
	int r = 0;
	char *end = NULL;
	errno = 0;

	r = strtoul(buffer, &end, 10);

	if (errno != 0) {
		fprintf(stderr, "Conversion error, %s\n", strerror(errno));
		exit (EXIT_FAILURE); 
	} else if (*end) {
		printf("Warning: converted partially: %i, non-convertible part: %s\n", r, end);
	}

	return r;
}

char *parse_char(char *buffer, int value) {
	sprintf(buffer, "%d", value);

	if (errno != 0) {
		fprintf(stderr, "Conversion error, %s\n", strerror(errno));
		exit (EXIT_FAILURE); 
	}

	return buffer;
}