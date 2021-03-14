#ifndef NEST_MSA_H
#define NEST_MSA_H
#define BLOCK_SIZE 64

//#define MAX_COL 120
#define MAX_COL 100
#define MAX_ROW 100



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

__device__ __host__ void pretty_print_matrix(Matrix *M);

Matrix create_peer_matrix(int strArrayLen, char **strArray);

__device__ __host__ double weight(Matrix *M, int row_index, int rowLen, bool test = false, double w1=0.25, double w2=0.5, double w3=1.0);

__device__ __host__ double objective(Matrix *M, int row_index, int end_index=-1, bool test=false);

__device__ __host__ bool full_row(Matrix *M, int row_index, int rowLen, bool test=false);

__device__ __host__ void remove_missing_rows(Matrix *M, Matrix *ret_M);

__device__ __host__  void getposition(int value, int rowindex, Matrix *M, Particle *particle_ret);

__device__ __host__ MostFrequent mostfrequent(Matrix *M, int row_index, int rowLen);

__device__ __host__ void fly_down(Particle *p, Matrix *M, Matrix *matrix_ret, int stride=1, bool test=false);

__device__ char *column(Matrix *M, int i);

__host__ __device__ bool aligned(Matrix *M, int row_index, int rowLen);

__host__ __device__ Swarm create_swarm(int index, Matrix *M);

__device__ bool criteria3(Particle *p, int new_index, Matrix *M);

__device__ bool criteria2(Particle *p, int threshold);

__device__ bool stopcriteria(Particle *p, int newindex, Matrix *M, int threshold=5, bool debug=false);

__device__ int skip_missing(Matrix *M, int col, int length);

__device__ __host__ void print_swarm(Swarm *s);

Particle *row_alignment(int index, Matrix M);

Matrix nest_msa_main(Matrix M);

Swarm *transfer_swarm(Swarm cpu_swarm);

__global__ void MyKernel(Matrix *M_copy_p_in, Matrix *gpu_matrix_matrix, Swarm *swarm, int index, GPU_Result **result_matrix, double g_value);

#endif
