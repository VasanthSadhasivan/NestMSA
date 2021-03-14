#include "nest_msa_gpu.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <algorithm>
#include <set>
#include <iterator>
#include "hip/hip_runtime.h"

__device__ __host__ void pretty_print_matrix(Matrix *M)
{
    for (int i = 0; i < (*M).num_rows; i++)
    {
        for (int j = 0; j < (*M).num_cols; j++)
        {
            if (j + 1 != (*M).num_cols)
                printf("%c ", (*M).matrix[i][j]);
            else
                printf("%c\n", (*M).matrix[i][j]);
        }
    }
    printf("\n");
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

__device__ __host__ double weight(Matrix *M, int row_index, int rowLen, bool test, double w1, double w2, double w3) {
    
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

__device__ __host__ double objective(Matrix *M, int row_index, int end_index, bool test) {
    double weights = 0;
    int A = 0;
    
    for(int i = row_index; i < (*M).num_rows; i ++){
        weights += weight(M, i, (*M).num_cols);
        
        if(aligned(M, i, (*M).num_cols)){
            A += 1;
        }
        
    }
    

    MostFrequent mf = mostfrequent(M, row_index, (*M).num_cols);
    
    if(end_index == -1){
        end_index = (*M).num_rows-1;
    }
    if(end_index >= (*M).num_rows){
        printf("End index exceed matrix size\n");
        return -1;
    }
    
    int gaps = 0;
    for(int i = row_index; i < end_index+1; i ++){
        for(int k = 0; k < (*M).num_cols; k++){
            if((*M).matrix[i][k] == '-'){
                gaps += 1;
            }
        }
    }
    /*if (row_index == 5 && end_index == 10){
        printf("Gaps: %d row_index: %d endindex: %d, A: %d, weights: %f, C: %d\n", gaps, row_index, end_index, A, weights, mf.freq);
        //pretty_print_matrix(M);
        //printf("\n");
    }*/
    //if (test)
    //    pretty_print_matrix(M);
    //printf("weights: %f, A: %d,  mf.freq: %d, gaps: %d\n", weights, A, mf.freq, gaps);
    weights = weights * A * mf.freq;
    weights = weights / (1 + gaps);
    
    //printf("gaps: %d\n", gaps);
    return weights;
}

__device__ __host__ bool full_row(Matrix *M, int row_index, int rowLen, bool test) {
    if (rowLen <= 0) {
        return false;
    }

    char first_c = (*M).matrix[row_index][0];
    for (int i = 1; i < rowLen; i++) {
        if (first_c != (*M).matrix[row_index][i]) {
            return false;
        }
    }
    if (first_c == '-' || first_c == '#') {
        return false;
    }
    return true;
}

__device__ __host__ void remove_missing_rows(Matrix *M, Matrix *ret_M) {
    (*ret_M).num_cols = (*M).num_cols;
    (*ret_M).num_rows = 0;
    for(int i = 0; i < (*M).num_rows; i++){
        int skip = 1;
        for(int j = 0; j < (*M).num_cols; j++){
            if((*M).matrix[i][j] != '#'){
                skip = 0;
                break;
            }
        }
        if(!skip){
            (*ret_M).num_rows += 1;
        }
    }

    for(int i = 0; i < (*ret_M).num_rows; i++){
        memcpy((*ret_M).matrix[i],(*M).matrix[i], MAX_COL);
    }
    return;
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

__device__ __host__ void getposition(int value, int rowindex, Matrix *M, Particle *particle_ret)
{
    int num = 0;
    char* row = (*M).matrix[rowindex];

    for (int i = 0; i < (*M).num_cols; i++)
    {
        if (row[i] == value)
            num++;
    } 

    num = 0;
    for (int i = 0; i < (*M).num_cols; i++)
    {
        if (row[i] == value)
        {
            (*particle_ret).pos.col[num] = i;
            num++;
        }
    } 

    
    (*particle_ret).pos.row = rowindex;
    (*particle_ret).pos.num_cols = num;

    (*particle_ret).value = value;
    (*particle_ret).best = (*particle_ret).pos;
}

__device__ __host__ MostFrequent mostfrequent(Matrix *M, int row_index, int rowLen) {
    int charToOccurences[256] = {0};
    for (int i = 0; i < rowLen; i++) {
        charToOccurences[(*M).matrix[row_index][i]] += 1;
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

__device__ __host__ void fly_down(Particle *p, Matrix *M, Matrix *matrix_ret, int stride, bool test)
{
    
    Matrix M_new;
    
    M_new.num_rows = (*M).num_rows + stride;
    M_new.num_cols = (*M).num_cols;
    
    for (int i = 0; i < (*M).num_rows; i++)
    {
        for (int j = 0; j < (*M).num_cols; j++)
        {
            M_new.matrix[i][j] = (*M).matrix[i][j];
        }
    }
    
    for (int i = (*M).num_rows; i < M_new.num_rows; i++)
    {
        for (int j = 0; j < (*M).num_cols; j++)
        {
            M_new.matrix[i][j] = '#';;
        }
    }
    
    for (int i = (M_new.num_rows - 1); i > ((*p).pos.row + stride - 1); i--)
    {
        for (int j = 0; j < M_new.num_cols; j++)
        {
            for (int k = 0; k < (*p).pos.num_cols; k++)
            {
                if (j == (*p).pos.col[k])
                {
                    M_new.matrix[i][j] = M_new.matrix[i - stride][j];
                }
            }
        }
    }
    
    for (int k = 0; k < (*p).pos.num_cols; k++)
    {
        
        for (int i = 0; i < stride; i++)
        {
            M_new.matrix[(*p).pos.row + i][(*p).pos.col[k]] = '-';
            
        }
    }
    
    //printf("Before remove_missing_rows:\n");
    //pretty_print_matrix(M_new);
    
    remove_missing_rows(&M_new, matrix_ret);

    //printf("After remove_missing_rows:\n");
    //pretty_print_matrix(return_value);
}

__device__ char* column(Matrix *M, int col_number) 
{
    char* c = new char[(*M).num_rows];
    
    for (int i = 0; i < (*M).num_rows; i++)
    {
        c[i] = (*M).matrix[i][col_number];
    }
    return c;
}

__device__ __host__ bool aligned(Matrix *M, int row_index, int num_cols) 
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
        if ((*M).matrix[row_index][i] == '-' || (*M).matrix[row_index][i] == '#'){
            if (first_empty_c == 0){
                first_empty_c = (*M).matrix[row_index][i];
            }else if(first_empty_c != (*M).matrix[row_index][i]){
                return false;
            }
        }else{
            if (first_c == 0){
                first_c = (*M).matrix[row_index][i];
            }else{
                if ((*M).matrix[row_index][i] != first_c){
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

__device__ __host__ void create_swarm(int index, Matrix *M, Swarm *swarm_ret) 
{
    int num_p = 0;
    char* row = (*M).matrix[index];
    char current_c;
    bool flag = false;

    for (int i = 0; i < (*M).num_cols; i++)
    {
        current_c = row[i];
        if (current_c == '#')
            break;
        for (int j = 0; j < num_p; j++)
        {
            if ((*swarm_ret).swarm[j].value == current_c)
            {
                flag = true;
                break;
            }  
        }           
        if (!flag)
        {
            getposition(current_c, index, M, &((*swarm_ret).swarm[num_p]));         
            num_p++;
        }
        flag = false;
    }

    (*swarm_ret).num_particles = num_p;
}


__device__ bool criteria2(Particle *p, int threshold)
{
    return (*p).updated > threshold;
}

__device__ bool criteria3(Particle *p, int new_index, Matrix *M)
{
    Particle new_particle;
    getposition((*p).value, new_index, M, &new_particle);
    return (*p).pos.num_cols != new_particle.pos.num_cols;
}

__device__ bool stopcriteria(Particle *p, int newindex, Matrix *M, int threshold, bool debug) {
    
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

__device__ int skip_missing(/*char *array*/ Matrix *M, int col, int length) {

    int size = 0;

    for (int i = 0; i < length; i++){
        char elem = (*M).matrix[i][col];
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


__device__ __host__ void print_swarm(Swarm *s)
{
    for (int i = 0; i < (*s).num_particles; i++)
    {
        printf("Particle %d:\n", i + 1);
        printf("\tValue:   %c\n", (*s).swarm[i].value);
        printf("\tUpdated:   %d\n", (*s).swarm[i].updated);
        printf("\tRow:     %d\n", (*s).swarm[i].pos.row);
        printf("\tColumns: [");
        for (int j = 0; j < (*s).swarm[i].pos.num_cols; j++)
        {
            if (j != (*s).swarm[i].pos.num_cols - 1)
            {   
                printf("%d, ", (*s).swarm[i].pos.col[j]);
            }
            else
            {
                printf("%d]\n\n", (*s).swarm[i].pos.col[j]);
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

GPU_Result **create_result_matrix(int size, int width){
    GPU_Result **list_of_result_arrays;
    GPU_Result **return_matrix;
    list_of_result_arrays = (GPU_Result **) malloc(sizeof(GPU_Result*) * size);
    for (int i = 0; i < size; i++){
        hipMalloc(&list_of_result_arrays[i], sizeof(GPU_Result) * width);
    } 
    hipMalloc(&return_matrix, sizeof(GPU_Result*) * size);
    hipMemcpy(return_matrix, list_of_result_arrays, sizeof(GPU_Result*) * size, hipMemcpyHostToDevice);

    return return_matrix;
}


Matrix *create_input_flattened_matrix(int size, int width, Matrix M){
    Matrix *return_matrix;
    hipMalloc(&return_matrix, sizeof(Matrix) * size * width);
    for(int i = 0; i < size * width; i++){
        hipMemcpy(return_matrix + i, &M, sizeof(Matrix), hipMemcpyHostToDevice);
    }
    return return_matrix;
}

GPU_Result **transfer_result_matrix(GPU_Result **gpu_result_matrix, int size, int width){
    GPU_Result **result_matrix = (GPU_Result **) malloc(sizeof(GPU_Result*) * size);
    GPU_Result *tempGPU_Result_p;
    hipMemcpy(result_matrix, gpu_result_matrix, sizeof(GPU_Result *) * size, hipMemcpyDeviceToHost);

    for (int i = 0; i < size; i++){
        tempGPU_Result_p = (GPU_Result *) malloc(sizeof(GPU_Result) * width);
        hipMemcpy(tempGPU_Result_p,result_matrix[i], sizeof(GPU_Result) * width, hipMemcpyDeviceToHost);
        result_matrix[i] = tempGPU_Result_p;
    }

    return result_matrix;
}

GPU_Result find_best_result(GPU_Result **result_matrix, int size, int width){
    GPU_Result best_result = result_matrix[0][0];

    for (int i = 0; i < size; i ++){
        for (int j = 0; j < width; j++){
            if (result_matrix[i][j].g_value > best_result.g_value){
                best_result = result_matrix[i][j];
            }
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
    GPU_Result **result_matrix;
    GPU_Result best_result;
    Particle *return_g;

    Swarm *gpu_swarm;
    Matrix *gpu_M;
    Matrix *gpu_input_matrix;
    GPU_Result **gpu_result_matrix;

    memset(&swarm, 0, sizeof(Swarm));
    memset(&g, 0, sizeof(Particle));
    memset(&best_result, 0, sizeof(GPU_Result));

    if (aligned(&M, index, M.num_cols)){
        return NULL;
    }

    
    create_swarm(index, &M, &swarm);
    
    g = swarm.swarm[0];

    original_g_value = g_value = objective(&M, index, index);
    
    gpu_swarm = transfer_swarm(swarm);

    gpu_M = transfer_M(M);
    gpu_result_matrix = create_result_matrix(swarm.num_particles, M.num_rows);
    gpu_input_matrix = create_input_flattened_matrix(swarm.num_particles, M.num_rows, M);
    
    if ((M.num_rows * swarm.num_particles) % BLOCK_SIZE == 0){
        hipLaunchKernelGGL(MyKernel, dim3((M.num_rows * swarm.num_particles)/BLOCK_SIZE), dim3(BLOCK_SIZE), 0, 0,
        gpu_M, gpu_input_matrix, gpu_swarm, index, gpu_result_matrix, g_value);
    }else{
        hipLaunchKernelGGL(MyKernel, dim3((M.num_rows * swarm.num_particles)/BLOCK_SIZE + 1), dim3(BLOCK_SIZE), 0, 0,
        gpu_M, gpu_input_matrix, gpu_swarm, index, gpu_result_matrix, g_value);
    }

    //if ( index==1){
        
    //        GPU_Result * test = (GPU_Result *)calloc(sizeof(GPU_Result), swarm.num_particles);
     //       printf("VALUE: %d %d\n", hipMemcpy(test, gpu_result_array, sizeof(GPU_Result) *swarm.num_particles, hipMemcpyDeviceToHost), hipSuccess);
     //       printf("globaly_optimal.pos.num_cols: %d, globaly_optimal.pos.row: %d globaly_optimal.value: %c\n", (test[0].g).pos.num_cols, (test[0].g).pos.row, (test[0].g).value);
            
     //       printf("Last Col: %d\n",(test[0].g).pos.col[2]);
            //printf("globaly_optimal.pos.num_cols: %d, globaly_optimal.pos.row: %d globaly_optimal.value: %c\n", (test[1].g).pos.num_cols, (test[1].g).pos.row, (test[1].g).value);
            //printf("Last Col: %d\n",(test[1].g).pos.col[2]);            
       //     while(1){}
    //}
    result_matrix = transfer_result_matrix(gpu_result_matrix, swarm.num_particles, M.num_rows);

    best_result = find_best_result(result_matrix, swarm.num_particles, M.num_rows);
    
    //printf("g_value: %f, index: %d\n", best_result.g_value, index);
    if (best_result.g_value == original_g_value){
        return NULL;
    }

    return_g = (Particle *) malloc(sizeof(Particle));
    memcpy(return_g, &(best_result.g), sizeof(Particle));
    
    return return_g;
}

__global__ void MyKernel(Matrix *M_copy_p_in, Matrix *gpu_input_matrix, Swarm *swarm, int index, GPU_Result **result_matrix, double g_value) {
    int thread_index;
    int thread_particle_index;
    int thread_flydown_index;
    Particle g;
    int index_copy = index;    
    

    thread_index = (hipThreadIdx_x + (hipBlockIdx_x * BLOCK_SIZE)) ;
    thread_particle_index = thread_index % swarm -> num_particles;
    thread_flydown_index = thread_index / swarm -> num_particles;
    Particle particle = swarm -> swarm[thread_particle_index];

    Matrix *M_copy_p = gpu_input_matrix + thread_index;

    if (thread_index >= swarm->num_particles * M_copy_p_in->num_rows){
        return;
    }
    
    particle.best_value = objective(M_copy_p, index, index_copy, thread_particle_index==1);
    
    int max_len = 0;

    for (int i=0; i < (*M_copy_p).num_cols; i++){

        if (!contains(i, particle.pos.col, particle.pos.num_cols)){
            //int temp_len = skip_missing(column(M_copy, i), M_copy.num_rows);
            int temp_len = skip_missing(M_copy_p, i, M_copy_p -> num_rows);
            
            if (temp_len > max_len){
                max_len = temp_len;
            }

        }
    }
    
    
    int criteria_1 = max_len;
    //if (thread_index == 0){
    //    printf("index: %d, thread_index: %d, particle.value: %c\n", index, thread_index, particle.value);
    //    print_swarm(swarm);
    //}
    
    //if (index == 3 && particle.value == 'b'){
    //    printf("criteria_1: %d, index: %d, index_copy: %d, particle: %c\n" ,criteria_1, index, index_copy, particle.value);
    //    pretty_print_matrix(M_copy);
    //}

    index_copy = index_copy + thread_flydown_index;
    //if (index == 3 && thread_index == 13){
    //    printf("particle.value: %c, index_copy: %d, particle.updated: %d, particle.best_value: %f\n", particle.value, index_copy, particle.updated, particle.best_value);
    //    pretty_print_matrix(M_copy_p);
    //    printf("cond: %d\n", stopcriteria(&particle, index_copy, M_copy_p));
    //}
    if (index_copy  < criteria_1-1 && !(stopcriteria(&particle, index_copy, M_copy_p))){
        
        index_copy += 1;
        particle.updated = thread_flydown_index + 1;
        fly_down(&particle, M_copy_p, M_copy_p, thread_flydown_index + 1);
        double score = objective(M_copy_p, index);
        
        
        
        if (score > g_value){
            Particle temp;
            g_value = score;
            g.value = particle.value;
            g.updated = particle.updated;
            g.pos = particle.pos;
            getposition(particle.value, index_copy, M_copy_p, &temp);
            g.best = temp.pos;
            g.best_value = score;
        }
    }
    
    /* while(index_copy < criteria_1-1 && !(stopcriteria(&particle, index_copy, &M_copy))){
        index_copy += 1;
        particle.updated += 1;
        
        fly_down(&particle, &M_copy, &M_copy);
        
        double score = objective(&M_copy, index);
        //printf("Score: %f\n", score);
        
        //if (index == 3 && particle.value == 'b'){
        //    printf("score: %f\n" , score);
        //    pretty_print_matrix(M_copy);
        //}
        if (score > particle.best_value){
            particle.best_value = score;
            particle.updated = 0;
        }

        if (score > g_value){
            Particle temp;
            g_value = score;
            g.value = particle.value;
            g.updated = particle.updated;
            g.pos = particle.pos;
            getposition(particle.value, index_copy, &M_copy, &temp);
            g.best = temp.pos;
            g.best_value = score;
            
        }
        
    } */
    
    result_matrix[thread_particle_index][thread_flydown_index].g_value = g_value;
    //memcpy(&result_matrix[thread_particle_index][thread_flydown_index].g_value, &g_value, sizeof(double));
    result_matrix[thread_particle_index][thread_flydown_index].g = g;
    //memcpy(&result_matrix[thread_particle_index][thread_flydown_index].g, &g, sizeof(Particle));
}

Matrix nest_msa_main(Matrix M){
    for (int i = 0; i < M.num_rows; i++){
        //pretty_print_matrix(M);
        Particle *globaly_optimal = row_alignment(i, M);

        if (globaly_optimal != NULL){
            fly_down(globaly_optimal, &M, &M, globaly_optimal->best.row - globaly_optimal->pos.row);
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
    pretty_print_matrix(&M);
    Matrix final = nest_msa_main(M);
    printf("\nAfter:\n");
    pretty_print_matrix(&final);
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