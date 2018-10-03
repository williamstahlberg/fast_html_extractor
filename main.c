#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void print_usage(char *name) {
	printf("Usage: %s input_file start middle end output_file\n", name);
	printf("Example: %s index.html '<div class=\"container\">' '<div' '</div>' extracted.html\n", name);
}

char *read_file(char *input_path) {
	char *text = NULL;
	FILE *fp = fopen(input_path, "r");

	if (fp != NULL) {
		fseek(fp, 0L, SEEK_END);
		long bufsize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		
		text = malloc(sizeof(char) * (bufsize + 1));
		size_t newLen = fread(text, sizeof(char), bufsize, fp);
		text[bufsize++] = '\0'; /* Just to be safe. */

		fclose(fp);
	}

	return text;
}

int main(int argc, char **argv) {
	if (argc < 5) {
		print_usage(argv[0]);
		exit(1);
	}

	char *input_path = argv[1];
	char *start = argv[2];
	char *middle = argv[3];
	char *end = argv[4];

	bool print_prog = false;
	FILE *output_fp = stdout;
	if (argc == 6) {
		output_fp = fopen(argv[5], "w+");
		print_prog = true;
	}

	if (print_prog) {
		fprintf(stderr, "Loading file...");
		fflush(stderr);
	}
	char *text = read_file(input_path);
	if (print_prog)
		fprintf(stderr, " Done.\n");

	size_t s_offset = strlen(start);
	size_t m_offset = strlen(middle);
	size_t e_offset = strlen(end);
	size_t text_len = strlen(text);

	int print_i = 0;
	int j = 0;
	while (j < text_len) {
		if (print_i % 5 == 0 && print_prog) {
			printf("                     ");
			printf("\rParsing: %.2f%%", 100.0*j/strlen(text));
			fflush(stdout);
		}

		char *t = text + j;  /* String we are working with. */
		int _i = strstr(t, start) - t; /* Find start tag. */

		if (_i < 0)
			break;

		int level = 1;
		int i = _i + s_offset;

		size_t t_len = strlen(t);
		while (i < t_len) {
			int i_[] = {
				strstr(t+i, middle) - (t+i),
				strstr(t+i, end) - (t+i)
			};

			if (i_[0] < i_[1] && i_[0] >= 0) {
				i += i_[0] + m_offset;
				level += 1;
			} else if (i_[1] >= 0) {
				i += i_[1] + e_offset;
				level -= 1; /* Should be 0 now. */
			} else {
				break;

			}

			if (level == 0) {
				fwrite(t+_i, i-_i, sizeof(char), output_fp);
				j += i;
				break;
			}
		}
		
		print_i++;
	}

	fclose(output_fp);

	return 0;
}
