#include "nest_msa_gpu.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <algorithm>
#include <set>
#include <iterator>
#include "hip/hip_runtime.h"

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
    Matrix M;

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


    for (int i = 0; i < strArrayLen; i++)
    {
        char* current_string = strArray[i];
        int len = lengths[i];
        for (int j = 0; j < len; j++)
        {
            M.matrix[j][i] = current_string[j];
        }
        if (len < num_rows)
        {
            for (int j = len; j < num_rows; j++)
            {
                M.matrix[j][i] = '#';
            }
        }   
    }
    
    M.num_cols = num_cols;
    M.num_rows = num_rows;
    return M;
}

__device__ __host__ double weight(Matrix M, int row_index, int rowLen, bool test, double w1, double w2, double w3) {
    
    if (full_row(M, row_index, rowLen, test)) {
        return w3;
    }
    
    int most_freq_count = mostfrequent(M, row_index, rowLen).freq;
    
    if (aligned(M, row_index, rowLen)) {
        return ((w2 * most_freq_count) / rowLen);
    }
    

    int x = most_freq_count;
    
    if (most_freq_count <= 1) {
        x = 0;
    }
    
    return ((w1 * x) / rowLen);
}

__device__ __host__ double objective(Matrix M, int row_index, int end_index, bool test) {
    double weights = 0;
    int A = 0;
    
    for(int i = row_index; i < M.num_rows; i ++){
        weights += weight(M, i, M.num_cols);
        
        if(aligned(M, i, M.num_cols)){
            A += 1;
        }
        
    }
    

    MostFrequent mf = mostfrequent(M, row_index, M.num_cols);
    
    if(end_index == -1){
        end_index = M.num_rows-1;
    }
    if(end_index >= M.num_rows){
        printf("End index exceed matrix size\n");
        return -1;
    }
    
    int gaps = 0;
    for(int i = row_index; i < end_index+1; i ++){
        for(int k = 0; k < M.num_cols; k++){
            if(M.matrix[i][k] == '-'){
                gaps += 1;
            }
        }
    }
    /*if (row_index == 5 && end_index == 10){
        printf("Gaps: %d row_index: %d endindex: %d, A: %d, weights: %f, C: %d\n", gaps, row_index, end_index, A, weights, mf.freq);
        //pretty_print_matrix(M);
        //printf("\n");
    }*/
    
    weights = weights * A * mf.freq;
    weights = weights / (1 + gaps);
    return weights;
}

__device__ __host__ bool full_row(Matrix M, int row_index, int rowLen, bool test) {
    if (rowLen <= 0) {
        return false;
    }

    char first_c = M.matrix[row_index][0];
    for (int i = 1; i < rowLen; i++) {
        if (first_c != M.matrix[row_index][i]) {
            return false;
        }
    }
    if (first_c == '-' || first_c == '#') {
        return false;
    }
    return true;
}

__device__ __host__ Matrix remove_missing_rows(Matrix M) {
    Matrix mat;

    mat.num_cols = M.num_cols;
    mat.num_rows = 0;
    for(int i = 0; i < M.num_rows; i++){
        int skip = 1;
        for(int j = 0; j < M.num_cols; j++){
            if(M.matrix[i][j] != '#'){
                skip = 0;
                break;
            }
        }
        if(!skip){
            mat.num_rows += 1;
        }
    }

    for(int i = 0; i < mat.num_rows; i++){
        memcpy(mat.matrix[i], M.matrix[i], MAX_COL);
    }
    return mat;
}
/*
Matrix remove_missing_rows(Matrix M) {
    Matrix mat;
    char** actualmat = new char*[M.num_rows];
    mat.num_cols = M.num_cols;
    mat.num_rows = 0;
    for(int i = 0; i < M.num_rows; i++){
        int skip = 1;
        for(int j = 0; j < M.num_cols; j++){
            if(M.matrix[i][j] != '#'){
                skip = 0;
                break;
            }
        }
        if(!skip){
            mat.num_rows += 1;
            actualmat[mat.num_rows-1] = new char[mat.num_cols];

            for(int j = 0; j < mat.num_cols; j++){
                actualmat[mat.num_rows-1][j] = M.matrix[mat.num_rows-1][j];
            }

        }
    }

    for (int i = 0; i < mat.num_rows; i++)
    {
        actualmat[i] = new char[mat.num_cols];
    }
    mat.matrix = actualmat;
    for(int i = 0; i < mat.num_rows; i++){
        mat.matrix[i] = M.matrix[i];
    }
    return mat;
}
*/

__device__ __host__ Particle getposition(int value, int rowindex, Matrix M)
{
    int num = 0;
    char* row = M.matrix[rowindex];
    Position p;

    for (int i = 0; i < M.num_cols; i++)
    {
        if (row[i] == value)
            num++;
    } 

    num = 0;
    for (int i = 0; i < M.num_cols; i++)
    {
        if (row[i] == value)
        {
            p.col[num] = i;
            num++;
        }
    } 

    
    p.row = rowindex;
    p.num_cols = num;

    Particle output_particle;
    output_particle.value = value;
    output_particle.pos = p;
    output_particle.best = p;
    return output_particle;
}

__device__ __host__ MostFrequent mostfrequent(Matrix M, int row_index, int rowLen) {
    int charToOccurences[256] = {0};
    for (int i = 0; i < rowLen; i++) {
        charToOccurences[M.matrix[row_index][i]] += 1;
    }

    int best_freq = 0;
    char best_freqChar = '\0';
    for (int i = 0; i < 256; i++) {
        if (charToOccurences[i] > best_freq) {
            best_freq = charToOccurences[i];
            best_freqChar = i;
        }
    }

    MostFrequent mf = {
        .freq = best_freq,
        .freqChar = best_freqChar
    };
    return mf;
}

__device__ __host__ Matrix fly_down(Particle p, Matrix M, int stride)
{
    Matrix M_new;

    M_new.num_rows = M.num_rows + stride;
    M_new.num_cols = M.num_cols;

    for (int i = 0; i < M.num_rows; i++)
    {
        for (int j = 0; j < M.num_cols; j++)
        {
            M_new.matrix[i][j] = M.matrix[i][j];
        }
    }
    for (int i = M.num_rows; i < M_new.num_rows; i++)
    {
        for (int j = 0; j < M.num_cols; j++)
        {
            M_new.matrix[i][j] = '#';;
        }
    }

    for (int i = (M_new.num_rows - 1); i > (p.pos.row + stride - 1); i--)
    {
        for (int j = 0; j < M_new.num_cols; j++)
        {
            for (int k = 0; k < p.pos.num_cols; k++)
            {
                if (j == p.pos.col[k])
                {
                    M_new.matrix[i][j] = M_new.matrix[i - stride][j];
                }
            }
        }
    }

    for (int k = 0; k < p.pos.num_cols; k++)
    {
        for (int i = 0; i < stride; i++)
        {
            M_new.matrix[p.pos.row + i][p.pos.col[k]] = '-';
        }
    }

    //printf("Before remove_missing_rows:\n");
    //pretty_print_matrix(M_new);

    Matrix return_value = remove_missing_rows(M_new);

    //printf("After remove_missing_rows:\n");
    //pretty_print_matrix(return_value);

    return return_value;
}

__device__ char* column(Matrix M, int col_number) 
{
    char* c = new char[M.num_rows];
    
    for (int i = 0; i < M.num_rows; i++)
    {
        c[i] = M.matrix[i][col_number];
    }
    return c;
}

__device__ __host__ bool aligned(Matrix M, int row_index, int num_cols) 
{
    char first_c = 0;//row[0];
    char first_empty_c = 0;
    /*int empty = 0;
    if(first_c == '#' || first_c == '-'){
        empty = 1;
    }
    for (int i = 1; i < num_cols; i++)
    {
        if (row[i] != first_c && row[i] != '-' && row[i] != '#' && !empty)
        {
            return false;
        }
         
    }*/

    for (int i =0; i<num_cols; i++){
        if (M.matrix[row_index][i] == '-' || M.matrix[row_index][i] == '#'){
            if (first_empty_c == 0){
                first_empty_c = M.matrix[row_index][i];
            }else if(first_empty_c != M.matrix[row_index][i]){
                return false;
            }
        }else{
            if (first_c == 0){
                first_c = M.matrix[row_index][i];
            }else{
                if (M.matrix[row_index][i] != first_c){
                    return false;
                }
            }
        }
    }

    if (first_c == 0){
        return false;
    }

    return true;
}

__device__ __host__ Swarm create_swarm(int index, Matrix M) 
{
    Swarm s;

    int num_p = 0;
    char* row = M.matrix[index];
    char current_c;
    bool flag = false;

    for (int i = 0; i < M.num_cols; i++)
    {
        current_c = row[i];
        if (current_c == '#')
            break;
        for (int j = 0; j < num_p; j++)
        {
            if (s.swarm[j].value == current_c)
            {
                flag = true;
                break;
            }  
        }           
        if (!flag)
        {
            s.swarm[num_p] = getposition(current_c, index, M);         
            num_p++;
        }
        flag = false;
    }

    s.num_particles = num_p;
    return s;
}


__device__ bool criteria2(Particle p, int threshold)
{
    return p.updated > threshold;
}

__device__ bool criteria3(Particle p, int new_index, Matrix M)
{
    Particle new_particle = getposition(p.value, new_index, M);
    return p.pos.num_cols != new_particle.pos.num_cols;
}

__device__ bool stopcriteria(Particle p, int newindex, Matrix M, int threshold, bool debug) {
    
    bool c2 = criteria2(p, threshold);
    
    bool c3 = criteria3(p, newindex, M);
    
    if (debug && c2) {
       printf("Terminating because of criteria 2\n");
    }
    else if (debug && c3) {
       printf("Terminating because of criteria 3\n");
    }
    return (c2 && c3);
}

__device__ int skip_missing(/*char *array*/ Matrix M, int col, int length) {

    int size = 0;

    for (int i = 0; i < length; i++){
        char elem = M.matrix[i][col];
        if (elem != '#'){
            size += 1;
        }
    }

    return size;
}

Matrix copyMatrix(Matrix M){
    int i, j;

    Matrix matrix_copy;
    matrix_copy.num_cols = M.num_cols;
    matrix_copy.num_rows = M.num_rows;

    for (i = 0; i < matrix_copy.num_rows; i++)
    {
        for (j = 0; j < matrix_copy.num_cols; j++)
        {
            matrix_copy.matrix[i][j] = M.matrix[i][j];
        }
    }

    return matrix_copy;
}


void print_swarm(Swarm s)
{
    for (int i = 0; i < s.num_particles; i++)
    {
        printf("Particle %d:\n", i + 1);
        printf("\tValue:   %c\n", s.swarm[i].value);
        printf("\tRow:     %d\n", s.swarm[i].pos.row);
        printf("\tColumns: [");
        for (int j = 0; j < s.swarm[i].pos.num_cols; j++)
        {
            if (j != s.swarm[i].pos.num_cols - 1)
            {   
                printf("%d, ", s.swarm[i].pos.col[j]);
            }
            else
            {
                printf("%d]\n\n", s.swarm[i].pos.col[j]);
            }
        }
    }
}

__device__ bool contains(int elem, int *list, int length){
    for(int i = 0; i < length; i++){
        if (list[i] == elem){
            return true;
        }
    }

    return false;
}

Swarm *transfer_swarm(Swarm cpu_swarm){
    Swarm *gpu_swarm;
    hipMalloc(&gpu_swarm, sizeof(Swarm));
    hipMemcpy(gpu_swarm, &cpu_swarm, sizeof(Swarm), hipMemcpyHostToDevice);
    return gpu_swarm;
}

Matrix *transfer_M(Matrix cpu_M){
    Matrix *gpu_M;
    hipMalloc(&gpu_M, sizeof(Matrix));
    hipMemcpy(gpu_M, &cpu_M, sizeof(Matrix), hipMemcpyHostToDevice);
    return gpu_M;
}

GPU_Result *create_result_array(int size){
    GPU_Result *result_array;
    hipMalloc(&result_array, sizeof(GPU_Result) * size);

    return result_array;
}

GPU_Result *transfer_result_array(GPU_Result *gpu_result_array, int size){
    GPU_Result *result_array = (GPU_Result *) malloc(sizeof(GPU_Result) * size);

    hipMemcpy(result_array, gpu_result_array, sizeof(GPU_Result) * size, hipMemcpyDeviceToHost);

    for (int i = 0; i < size; i++){
        hipMemcpy(result_array[i].g.pos.col, result_array[i].g.pos.col, sizeof(int) * result_array[i].g.pos.num_cols, hipMemcpyDeviceToHost);

        hipMemcpy(result_array[i].g.best.col, result_array[i].g.best.col, sizeof(int) * result_array[i].g.best.num_cols, hipMemcpyDeviceToHost);
    }

    return result_array;
}

GPU_Result find_best_result(GPU_Result *result_array, int size){
    GPU_Result best_result = result_array[0];

    for (int i = 1; i < size; i ++){
        if (result_array[i].g_value > best_result.g_value){
            best_result = result_array[i];
        }
    }

    return best_result;
}


Particle *row_alignment(int index, Matrix M){
    char *row = M.matrix[index];
    int index_copy;
    Swarm swarm;
    Particle g;
    float g_value;
    float original_g_value;
    GPU_Result *result_array;
    GPU_Result best_result;
    Particle *return_g;

    Swarm *gpu_swarm;
    Matrix *gpu_M;
    GPU_Result *gpu_result_array;

    if (aligned(M, index, M.num_cols)){
        return NULL;
    }

    swarm = create_swarm(index, M);
    g = swarm.swarm[0];

    original_g_value = g_value = objective(M, index, index);

    gpu_swarm = transfer_swarm(swarm);
    gpu_M = transfer_M(M);
    gpu_result_array = create_result_array(swarm.num_particles);
    
    if (swarm.num_particles % BLOCK_SIZE == 0) {
        hipLaunchKernelGGL(MyKernel, dim3(swarm.num_particles/BLOCK_SIZE), dim3(BLOCK_SIZE), 0, 0,
        gpu_M, gpu_swarm, index, gpu_result_array, g_value, swarm.num_particles);
    } else{
        hipLaunchKernelGGL(MyKernel, dim3(swarm.num_particles/BLOCK_SIZE + 1), dim3(BLOCK_SIZE), 0, 0,
        gpu_M, gpu_swarm, index, gpu_result_array, g_value, swarm.num_particles);
    }

    result_array = transfer_result_array(gpu_result_array, swarm.num_particles);

    best_result = find_best_result(result_array, swarm.num_particles);

    if (best_result.g_value == original_g_value){
        return NULL;
    }

    return_g = (Particle *) malloc(sizeof(Particle));
    memcpy(return_g, &(best_result.g), sizeof(Particle));
    return return_g;
}

__global__ void MyKernel(Matrix *M_copy_p, Swarm *swarm, int index, GPU_Result *result_array, double g_value, int max_threads) {
    //TODO Set thread index
    int thread_index;
    Particle particle = swarm -> swarm[thread_index];
    Particle g;
    int index_copy = index;
    Matrix M_copy = *M_copy_p;
    
    thread_index = hipThreadIdx_x + (hipBlockIdx_x * BLOCK_SIZE);
    
    if (thread_index >= max_threads){
        return;
    }

    particle.best_value = objective(M_copy, index, index_copy, true);
    
    int max_len = 0;

    for (int i=0; i < M_copy.num_cols; i++){

        if (!contains(i, particle.pos.col, particle.pos.num_cols)){

    
            //int temp_len = skip_missing(column(M_copy, i), M_copy.num_rows);
            int temp_len = skip_missing(M_copy, i, M_copy.num_rows);
            
            if (temp_len > max_len){
                max_len = temp_len;
            }

        }
    }
    

    int criteria_1 = max_len;
    

    
    while(index_copy < criteria_1-1 && !(stopcriteria(particle, index_copy, M_copy))){
        index_copy += 1;
        particle.updated += 1;
        
        M_copy = fly_down(particle, M_copy);

        double score = objective(M_copy, index);

        if (score > particle.best_value){
            particle.best_value = score;
            particle.updated = 0;
        }

        if (score > g_value){
            g_value = score;
            g.value = particle.value;
            g.updated = particle.updated;
            g.pos = particle.pos;
            g.best = getposition(particle.value, index_copy, M_copy).pos;
            g.best_value = score;
        }
    }
    result_array[thread_index].g_value = g_value;
    result_array[thread_index].g = g;
}

Matrix nest_msa_main(Matrix M){
    for (int i = 0; i < M.num_rows; i++){
        Particle *globaly_optimal = row_alignment(i, M);

        if (globaly_optimal != NULL){
            M = fly_down(*globaly_optimal, M, globaly_optimal->best.row - globaly_optimal->pos.row);
        }
    }

    return M;
}


int main(){
    const char *sequences[5];
    sequences[0] = "abbccdd";
    sequences[1] = "abccdd";
    sequences[2] = "abcdd";
    sequences[3] = "aabccdd";
    sequences[4] = "aabccc";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    printf("Before:\n");
    pretty_print_matrix(M);
    Matrix final = nest_msa_main(M);
    printf("\nAfter:\n");
    pretty_print_matrix(final);
    return 0;
}


/*
int main(){
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    printf("Before row alignment:\n");
    pretty_print_matrix(M);
    Particle *p = row_alignment(1, M);

    printf("Particle %c, Position: [%d], Best: (%d, [%d, %d, %d, %d])\n",
        p -> value,
        p -> pos.col[0],
        p -> best.row,
        p -> best.col[0],
        p -> best.col[1],
        p -> best.col[2],
        p -> best.col[3]
    );
    return 0;
}*/

/*
int main(){
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    double o = objective(M, 1);
    printf("O: %f\n", o);
    pretty_print_matrix(M);
    return 0;
}
*/
/*
int main(int argc, char** argv)
{
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char**)sequences);
    
    Particle p;
    p.value = 'b';
    Position p_pos;
    p_pos.row = 1;
    p_pos.num_cols = 3;
    p_pos.col = (int[3]) {0, 2, 3};
    p.pos = p_pos;


    pretty_print_matrix(M);
    printf("\n\n");
    Matrix test = fly_down(p, M, 1);
    printf("Number of Rows: %d\n", test.num_rows);
    printf("\n\n");
    pretty_print_matrix(test);
    

}
*/