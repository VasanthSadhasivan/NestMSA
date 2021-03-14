#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nest_msa.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect usage\n");
        return EXIT_FAILURE;
    }

    FILE *file = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return EXIT_FAILURE;
    }

    char *sequences[120];
    int i = 0;
    while (((read = getline(&line, &len, file)) != -1) && (i < 120)) {
        line[read-1] = '\0';
        sequences[i] = (char *)malloc(read);
        if (sequences[i] == NULL) {
            printf("malloc() error\n");
            return EXIT_FAILURE;
        }
        strcpy(sequences[i], line);
        i++;
    }

    fclose(file);

    int num_of_sequences = i;
    Matrix M = create_peer_matrix(num_of_sequences, (char **)sequences);

    Matrix final = nest_msa_main(M);
}
