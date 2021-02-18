#ifndef NEST_MSA_H
#define NEST_MSA_H

struct Matrix
{
    int num_cols;
    int num_rows;
    char** matrix;
};

struct Position
{
    int row;
    int num_cols;
    int* col;
}; 

struct Particle
{
    int value;
    int updated;
    Position pos;
    Position best;
    double best_value;
};

#endif
