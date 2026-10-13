#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_LEN 2048
#define MAX_LINE 256

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "No input file provided.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Failed to open input file.\n");
        exit(EXIT_FAILURE);
    }

    char source[MAX_FILE_LEN] = {0};
    char buffer[MAX_LINE + 1];

    int ln_c = 0;
    while (fgets(buffer, MAX_LINE, fp) != NULL) {

        strcat(source, buffer);
        ln_c++;
    }

    fclose(fp);
    return 0;
}
