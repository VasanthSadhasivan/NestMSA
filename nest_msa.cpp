#include "nest_msa.h"
#include <stdio.h>
#include <string.h>
#include <map>


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
    if (full_row(row, rowLen)) {
        return w3;
    }

    int most_freq_count = mostfrequent(row, rowLen).freq;

    if (aligned(row, rowLen)) {
        return ((w2 * most_freq_count) / rowLen);
    }

    int x = most_freq_count;
    if (most_freq_count <= 1) {
        x = 0;
    }
    return ((w1 * x) / rowLen);
}


double objective(Matrix M, int row_index, int end_index) {
    double weights = 0;
    int A = 0;
    for(int i = row_index; i < M.num_rows; i ++){
        weights += weight(M.matrix[i], M.num_cols);
        if(aligned(M.matrix[i], M.num_cols)){
            A += 1;
        }
    }
    MostFrequent mf = mostfrequent(M.matrix[row_index], M.num_cols);
    
    if(end_index == -1){
        end_index = M.num_cols;
    }
    if(end_index > M.num_cols){
        printf("End index exceed matrix size\n");
        exit(1);
    }
    int gaps = 0;
    for(int i = row_index; i < M.num_rows; i ++){
        int row_emp = M.num_cols;
        for(int k = 0; k < M.num_cols; k++){
            if(M.matrix[i][k] == '#'){
                row_emp -= 1;
            }
        }
        if(row_emp == 0){
            gaps += 1;
        }
    }
    weights = weights * A * mf.freq;
    weights = weights / (1 + gaps);
    return weights;
}

bool full_row(char *row, int rowLen) {
    if (rowLen <= 0) {
        return false;
    }
    char first_c = row[0];
    for (int i = 1; i < rowLen; i++) {
        if (first_c != row[i]) {
            return false;
        }
    }
    if (first_c == '-' || first_c == '#') {
        return false;
    }
    return true;
}

Matrix remove_missing_rows(Matrix M) {
    Matrix mat;

    mat.num_cols = M.num_cols;
    mat.num_rows = 0;
    for(int i = 0; i < M.num_rows; i++){
        int skip = 1;
        for(int j = 0; j < M.num_cols; j++){
            if(M.matrix[i][j] != '#' && M.matrix[i][j] != '-'){
                skip = 0;
                break;
            }
        }
        if(!skip){
            mat.num_rows += 1;
        }
    }
    char** actualmat = new char*[mat.num_rows];
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
    std::map<char, int> hashmap;
    for (int i = 0; i < rowLen; i++) {
        if (hashmap.find(row[i]) == hashmap.end()) {
            hashmap[row[i]] = 1;
        }
        else {
            hashmap[row[i]]++;
        }
    }
    int best_freq = 0;
    char best_freqChar = '\0';
    std::map<char, int>::iterator it = hashmap.begin();
    while (it != hashmap.end()) {
        if ((it->second) > best_freq) {
            best_freq = it->second;
            best_freqChar = it->first;
        }
        it++;
    }
    MostFrequent mf = {
        .freq = best_freq,
        .freqChar = best_freqChar
    };
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
    for (int i = 0; i < M.num_rows; i++)
    {
        for (int j = 0; j < M.num_cols; j++)
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
                if (j == p.pos.col[k])
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
            M_new.matrix[p.pos.row + i][p.pos.col[k]] = '-';
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
    char first_c = row[0];
    int empty = 0;
    if(first_c == '#'){
        empty = 1;
    }
    for (int i = 1; i < num_cols; i++)
    {
        if (row[i] != first_c && row[i] != '-' && row[i] != '#' && !empty)
        {
            return false;
        }
         
    }
    return true;
}

Swarm create_swarm(int index, Matrix M) 
{
    Particle* swarm = new Particle[M.num_cols];
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
            if (swarm[j].value == current_c)
            {
                flag = true;
                break;
            }  
        }           
        if (!flag)
        {
            swarm[num_p] = getposition(current_c, index, M);         
            num_p++;
        }
        flag = false;
    }

    Swarm s = {
        .num_particles = num_p,
        .swarm = swarm
    };
    return s;
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
    bool c2 = criteria2(p, threshold);
    bool c3 = criteria3(p, newindex, M);
    if (debug && c2) {
        // print statement?
    }
    else if (debug && c3) {
        // print statement?
    }
    return (c2 && c3);
}

Matrix skip_missing(Matrix M) {
    Matrix mat;
    return mat;
}

Particle row_alignment(int index, Matrix M) {
    Particle p;
    return p;
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
    p_pos.col = new int[3] {0, 2, 3};
    p.pos = p_pos;


    pretty_print_matrix(M);
    printf("\n\n");
    Matrix test = fly_down(p, M, 3);
    printf("\n\n");
    pretty_print_matrix(test);
    

}
*/
