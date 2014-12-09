#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <packedobjects/packedobjects.h>

#include "functions.h"

#define MAX_CHARACTER 256

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

void parse_char(char *buffer, int value) {
	sprintf(buffer, "%d", value);

	if (errno != 0) {
		fprintf(stderr, "Conversion error, %s\n", strerror(errno));
		exit (EXIT_FAILURE); 
	}
}

void parse_currency(char *buffer) {
	if (strchr(buffer, '.') == NULL) {
		int i = 0;
		i = parse_int(buffer);

		printf("Warning: converted partially: %s, adding .00 at the end of the currency\n", buffer);
		strcat(buffer, ".00");
	} else {
		int len = strlen(strchr(buffer, '.'));
		
		if (len == 1) {
			printf("Warning: converted partially: %s, adding 00 at the end of the currency\n", buffer);
			strcat(buffer, "00");
		}
		if (len == 2) {
			printf("Warning: converted partially: %s, adding 0 at the end of the currency\n", buffer);
			strcat(buffer, "0");
		} 
		if (len >= 4) {
			int l = len - 3;
			int ls = strlen(buffer);

			buffer[ls - l] = '\0';

			printf("Warning: converted partially: %s, removing subsequent numbers\n", buffer);
		}
	}
}

void parse_decimal(char *buffer) {
	if (strchr(buffer, '.') == NULL) {
		int i = 0;
		i = parse_int(buffer);

		printf("Warning: converted partially: %s, adding .0 at the end of the currency\n", buffer);
		strcat(buffer, ".0");
	} else {
		int len = strlen(strchr(buffer, '.'));
		
		if (len == 1) {
			printf("Warning: converted partially: %s, adding 0 at the end of the currency\n", buffer);
			strcat(buffer, "0");
		}
		if (len >= 3) {
			int l = len - 2;
			int ls = strlen(buffer);

			buffer[ls - l] = '\0';

			printf("Warning: converted partially: %s, removing subsequent numbers\n", buffer);
		}
	}
}

void parse_boolean(char *buffer) {
	if (strcmp(buffer, "TRUE") == 0 || strcmp(buffer, "FALSE") == 0) {
		printf("Warning: converted partially: %s, transforming in lower case\n", buffer);
		toLowerCase(buffer);
	} else if (strcmp(buffer, "1") == 0) {
	 	printf("Warning: converting: %s in true\n", buffer);
	 	strcpy(buffer, "true");
	 } else if (strcmp(buffer, "0") == 0) {
	 	printf("Warning: converting: %s in false\n", buffer);
	 	strcpy(buffer, "false");
	} else if (strcmp(buffer, "true") == 0 || strcmp(buffer, "false") == 0) {
		//nothing to be done due to value inserted be in correct form
	} else {
		printf("Conversion error, couldn't convert: %s\n", buffer);
		exit(EXIT_FAILURE);
	}
}

void parse_departament(char *buffer) {

}