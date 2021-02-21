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

Particle getposition(int value, int rowindex, Matrix M)
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

    Particle output_particle;
    output_particle.value = value;
    output_particle.pos = p;
    return output_particle;
}

MostFrequent mostfrequent(char *row, int rowLen) {
    MostFrequent mf;
    return mf;
}

Matrix fly_down(Particle p, Matrix M, int stride) 
{
    Matrix M_new;

    M_new.num_rows = M.num_rows + stride;
    M_new.num_cols = M.num_cols;
    char** mat = new char*[M_new.num_rows];
    for (int i = 0; i < M_new.num_rows; i++)
    {
        mat[i] = new char[M_new.num_cols];
    }
    M_new.matrix = mat;
    for (int i = 0; i < M.num_cols; i++)
    {
        for (int j = 0; j < M.num_rows; j++)
        {
            M_new.matrix[i][j] = M.matrix[i][j];
        }
    }
    for (int i = M.num_rows; i < M_new.num_rows; i++)
    {
        for (int j = 0; j < M.num_rows; j++)
        {
            M_new.matrix[i][j] = '#';;
        }
    }

    for (int i = (M_new.num_rows - 1); i > p.pos.row; i--)
    {
        for (int j = 0; j < M_new.num_cols; j++)
        {
            for (int k = 0; k < p.pos.num_cols; k++)
            {
                if (j == k)
                {
                    M_new.matrix[i][j] = M_new.matrix[i - 1][j];
                }
            }
        }
    }

    for (int k = 0; k < p.pos.num_cols; k++)
    {
        for (int i = 0; i < stride; i++)
        {
            M_new.matrix[p.pos.row + i][k] = '-';
        }
    }

    return remove_missing_rows(M_new);
}

char* column(Matrix M, int col_number) 
{
    char* column = new char[M.num_rows];
    for (int i = 0; i < M.num_rows; i++)
    {
        column[i] = M.matrix[i][col_number];
    }
    return column;
}

bool aligned(char *row, int num_cols) 
{
    bool output;
    char first_c = row[0];
    for (int i = 1; i < num_cols; i++)
    {
        if (row[i] != first_c and row[i] != '-');
        {
            return false;
        }
         
    }
    return true;
}

Particle* clip_swarm(Particle* swarm, int num_particles)
{
    Particle* new_swarm = new Particle[num_particles];

    MostFrequent mf;
    
    for (int i = 0; i < num_particles; i++)
    {
        new_swarm[i] = swarm[i];
    }

    delete swarm;
    return new_swarm;
}

Particle* create_swarm(int index, Matrix M) 
{
    Particle* swarm = new Particle[M.num_cols];
    int num_p = 0;
    char* row = M.matrix[index];
    char current_c;
    bool flag = true;

    for (int i = 0; i < M.num_cols; i++)
    {
        current_c = row[i];
        for (int j = 0; j < num_p or flag; j++)
        {
            if (swarm[j].value == current_c)
            {
                flag = false;
            }  
        }           
        if (flag)
        {
            swarm[num_p] = getposition(current_c, index, M);         
            num_p++;
        }
        flag = true;
    }

    Particle* new_swarm = clip_swarm(swarm, num_p);
    return new_swarm;
}


bool criteria2(Particle p, int threshold)
{
    return p.updated > threshold;
}

bool criteria3(Particle p, int new_index, Matrix M)
{
    Particle new_particle = getposition(p.value, new_index, M);
    return p.pos.num_cols != new_particle.pos.num_cols;
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
