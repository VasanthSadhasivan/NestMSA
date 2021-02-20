#include "nest_msa.h"
#include <stdio.h>
#include <string.h>

void pretty_print_matrix(Matrix M)
{
    for (int i = 0; i < M.num_rows; i++)
    {
        for (int j = 0; j < M.num_cols; j++)
        {
            if (j + 1 != M.num_cols)
                printf("%c ", M.matrix[i][j]);
            else
                printf("%c\n", M.matrix[i][j]);
        }
    }
}

Matrix create_peer_matrix(int strArrayLen, char **strArray)
{
    int num_cols = strArrayLen;
    int* lengths = new int[num_cols];
    for (int i = 0; i < strArrayLen; i++)
    {
        lengths[i] = strlen(strArray[i]);
    }
    int num_rows = lengths[0];
    for (int i = 1; i < num_cols; i++)
    {
        if (lengths[i] > num_rows)
            num_rows = lengths[i];
    }

    char** mat = new char*[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        mat[i] = new char[num_cols];
    }
    for (int i = 0; i < strArrayLen; i++)
    {
        char* current_string = strArray[i];
        int len = lengths[i];
        for (int j = 0; j < len; j++)
        {
            mat[j][i] = current_string[j];
        }
        if (len < num_rows)
        {
            for (int j = len; j < num_rows; j++)
            {
                mat[j][i] = '#';
            }
        }   
    }
    Matrix M;
    M.num_cols = num_cols;
    M.num_rows = num_rows;
    M.matrix = mat;
    return M;
}

double weight(char *row, int rowLen, double w1, double w2, double w3) {
    return 0;
}

double objective(Matrix M, int row_index, int end_index) {
    return 0;
}

bool full_row(char *row, int rowLen) {
    return false;
}

Matrix remove_missing_rows(Matrix M) {
    Matrix mat;
    return mat;
}

Position getposition(int value, int rowindex, Matrix M)
{
    int num = 0;
    char* row = M.matrix[rowindex];
    for (int i = 0; i < M.num_cols; i++)
    {
        if (row[i] == value)
            num++;
    } 

    int* indices = new int[num];
    num = 0;
    for (int i = 0; i < M.num_cols; i++)
    {
        if (row[i] == value)
        {
            indices[num] = i;
            num++;
        }
    } 

    Position p;
    p.row = rowindex;
    p.num_cols = num;
    p.col = indices;
    return p;
}

MostFrequent mostfrequent(char *row, int rowLen) {
    MostFrequent mf;
    return mf;
}

Matrix fly_down(Particle p, Matrix M, int stride) {
    Matrix mat;
    return mat;
}

char *column(Matrix M, int i) {
    char *c;
    return c;
}

bool aligned(char *row, int rowLen) {
    return false;
}

Particle *create_swarm(int index, Matrix M) {
    Particle *p;
    return p;
}

bool criteria2(Particle p, int threshold)
{
    return p.updated > threshold;
}

bool criteria3(Particle p, int new_index, Matrix M)
{
    Position new_pos = getposition(p.value, new_index, M);
    return p.pos.num_cols != new_pos.num_cols;
}

bool stopcriteria(Particle p, int newindex, Matrix M, int threshold, bool debug) {
    return false;
}

Matrix skip_missing(Matrix M) {
    Matrix mat;
    return mat;
}

Particle row_alignment(int index, Matrix M) {
    Particle p;
    return p;
}
