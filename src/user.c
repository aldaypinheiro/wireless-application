#include <stdio.h>

#define MAX_CHARACTER 256

char *user_input(char *buffer, char *node_name) {
	printf("Enter the %s:\n", node_name);
	if (fgets(buffer, MAX_CHARACTER, stdin) == NULL) {
		fprintf(stderr, "failed to read string\n");
		return NULL;
	}
	strtok(buffer, "\n");
	return buffer;
}