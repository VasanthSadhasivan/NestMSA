#ifndef NEST_MSA_H
#define NEST_MSA_H
#define BLOCK_SIZE 256

#define MAX_COL 10
#define MAX_ROW 10



struct Matrix
{
    int num_cols;
    int num_rows;
    char matrix[MAX_ROW][MAX_COL];
};

struct Position
{
    int row;
    int num_cols;
    int col[MAX_COL];
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
    Particle swarm[MAX_COL];
};

void pretty_print_matrix(Matrix M);

Matrix create_peer_matrix(int strArrayLen, char **strArray);

__device__ __host__ double weight(Matrix M, int row_index, int rowLen, bool test = false, double w1=0.25, double w2=0.5, double w3=1.0);

__device__ __host__ double objective(Matrix M, int row_index, int end_index=-1, bool test=false);

__device__ __host__ bool full_row(Matrix M, int row_index, int rowLen, bool test=false);

__device__ __host__ Matrix remove_missing_rows(Matrix M);

__device__ __host__  Particle getposition(int value, int rowindex, Matrix M);

__device__ __host__ MostFrequent mostfrequent(Matrix M, int row_index, int rowLen);

__device__ __host__ Matrix fly_down(Particle p, Matrix M, int stride=1);

__device__ char *column(Matrix M, int i);

__host__ __device__ bool aligned(Matrix M, int row_index, int rowLen);

__host__ __device__ Swarm create_swarm(int index, Matrix M);

__device__ bool criteria3(Particle p, int new_index, Matrix M);

__device__ bool criteria2(Particle p, int threshold);

__device__ bool stopcriteria(Particle p, int newindex, Matrix M, int threshold=5, bool debug=false);

__device__ int skip_missing(Matrix M, int col, int length);

Particle *row_alignment(int index, Matrix M);

Matrix nest_msa_main(Matrix M);

Swarm *transfer_swarm(Swarm cpu_swarm);

__global__ void MyKernel(Matrix *M_copy_p, Swarm *swarm, int index, GPU_Result *result_array, double g_value, int max_threads);

#endif
