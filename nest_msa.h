#ifndef NEST_MSA_H
#define NEST_MSA_H
#define BLOCK_SIZE 256

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

typedef struct GPU_Res
{
    Particle g;
    double g_value;
}GPU_Result;

struct MostFrequent
{
    int freq;
    char freqChar;
};

struct Swarm
{
    int num_particles;
    Particle *swarm;
};

void pretty_print_matrix(Matrix M);

Matrix create_peer_matrix(int strArrayLen, char **strArray);

double weight(char *row, int rowLen, double w1=0.25, double w2=0.5, double w3=1.0);

double objective(Matrix M, int row_index, int end_index=-1);

bool full_row(char *row, int rowLen);

Matrix remove_missing_rows(Matrix M);

Particle getposition(int value, int rowindex, Matrix M);

MostFrequent mostfrequent(char *row, int rowLen);

Matrix fly_down(Particle p, Matrix M, int stride=1);

char *column(Matrix M, int i);

bool aligned(char *row, int rowLen);

Swarm create_swarm(int index, Matrix M);

bool criteria3(Particle p, int new_index, Matrix M);

bool criteria2(Particle p, int threshold);

bool stopcriteria(Particle p, int newindex, Matrix M, int threshold=5, bool debug=false);

int skip_missing(Matrix M);

Particle *row_alignment(int index, Matrix M);

Matrix nest_msa_main(Matrix M);

#endif
