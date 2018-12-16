#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#include "instructions.h"

int compile(FILE*, FILE*);





// a location to store global compile error
// messages
char *compile_err = NULL;

int main(int argc, char **argv) {

	FILE *in = stdin;
	if (argc > 1) {
		in = fopen(argv[1], "rb");
	}

	if (compile(in, stdout)) {
		fprintf(stderr, "Compile Error: %s\n", compile_err);
		exit(-1);
	}
	return 0;
}




typedef struct basic_block {
	char *name;

} basic_block_t;



// compile takes an in file and an out file
// and returns a 0 if it succeeded, 1 if not
// it will store error strings in compile_err
// global (such good style)
int compile(FILE *in, FILE *out) {
	char line[255];

	while (fgets(line, sizeof line, in)) {
			size_t len = strlen(line);
			if (len && (line[len - 1] != '\n')) {
				compile_err = "line too long to parse";
				return 1;
			}


			char *cleaned = calloc(1, len + 1);


			int o = 0;


			int prev_sp = 0;
			int encountered_non_space = 0;
			for (int i = 0; line[i]; i++) {
				switch (line[i]) {
					case '\n':
						continue;
					case '\t':
						line[i] = ' ';
						break;
				}

				if (line[i] == ';') break;

				if (line[i] == ' ') {

					if (prev_sp == 0 && encountered_non_space)
						cleaned[o++] = ' ';
					prev_sp = 1;
				} else {
					cleaned[o++] = line[i];
					encountered_non_space = 1;
					prev_sp = 0;
				}
			}

			char *src = cleaned;

			// skip over the first whitespace chars
			while(*src == '\t' || *src == ' ') src++;


			int srclen = strlen(src);

			// so now we have the cleaned up line
			// source, so now we need to parse it
			char *op = NULL;
			int arg = 0;

			if (srclen == 0) continue;


			// check for the start of a basic block
			if (src[srclen-1] == ':') {
				printf("%s\n", src);
			} else {
				printf("\t%s\n", src);
			}

			free(cleaned);
			/* possibly remove trailing newline ... and */
			/* deal with line */
	}
	return 0;
}
