#include "nest_msa.h"
#include <stdio.h>

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

int length(char* string)
{
    int cnt = 0;
    for (char* pnt = string; *pnt != '\0'; pnt++)
        cnt++;
    return cnt;
}

Matrix create_peer_matrix(int argc, char** argv)
{
    int num_cols = argc - 1;
    int* lengths = new int[num_cols];
    for (int i = 1; i < argc; i++)
    {
        lengths[i - 1] = length(argv[i]);
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
    for (int i = 1; i < argc; i++)
    {
        char* current_string = argv[i];
        int len = lengths[i - 1];
        for (int j = 0; j < len; j++)
        {
            mat[j][i - 1] = current_string[j];
        }
        if (len < num_rows)
        {
            for (int j = len; j < num_rows; j++)
            {
                mat[j][i - 1] = '-';
            }
        }   
    }
    Matrix M;
    M.num_cols = num_cols;
    M.num_rows = num_rows;
    M.matrix = mat;
    return M;
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

bool criteria2(Particle p, int threshold)
{
    return p.updated > threshold;
}

bool criteria3(Particle p, int new_index, Matrix M)
{
    Position new_pos = getposition(p.value, new_index, M);
    return p.pos.num_cols != new_pos.num_cols;
}

int main(int argc, char** argv)
{
    Matrix M;
    M = create_peer_matrix(argc, argv);
    pretty_print_matrix(M);
}

