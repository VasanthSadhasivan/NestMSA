#include "nest_msa.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <algorithm>
#include <set>
#include <iterator>

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

double objective(Matrix M, int row_index, int end_index=-1) {
    return 0;
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

Matrix fly_down(Particle p, Matrix M, int stride=1)
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
    char first_c = row[0];
    for (int i = 1; i < num_cols; i++)
    {
        if (row[i] != first_c && row[i] != '-' && row[i] != '#')
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

bool stopcriteria(Particle p, int newindex, Matrix M, int threshold, bool debug = false) {
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

std::vector<char> skip_missing(char **array) {
    std::vector<char> without_missing = {};
    for (char elem : array){
        if (elem != '#'){
            without_missing.push_back(elem);
        }
    }

    return without_missing;
}

Matrix copyMatrix(Matrix M){
    int i, j;

    Matrix matrix_copy;
    matrix_copy.num_cols = M.num_cols;
    matrix_copy.num_rows = M.num_rows;

    matrix_copy.matrix = new char*[M.num_rows];

    for (i = 0; i < matrix_copy.num_rows; i++)
    {
        matrix_copy.matrix[i] = new char[M_new.num_cols];
    }
    for (i = 0; i < matrix_copy.num_rows; i++)
    {
        for (j = 0; j < matrix_copy.num_cols; j++)
        {
            matrix_copy.matrix[i][j] = M.matrix[i][j];
        }
    }

    return matrix_copy;
}



Position *row_alignment(int index, Matrix M){
    char *row = M.matrix[index];
    int index_copy;

    if aligned(row, M.num_cols){
        return NULL;
    }

    Swarm swarm = create_swarm(index, M);

    Particle g = swarm.swarm[0];

    float original_g_value = g_value = objective(M, index, end_index = index);

    for (Particle particle : swarm.swarm){
        index_copy = index;
        Matrix M_copy = copyMatrix(M);

        particle.best_value = objective(M, index, end_index=index_copy);

        int max_len = 0;

        for (int i=0; i < M_new.num_cols; i++){
            if (std::find(std::begin(particle.pos.col), std::end(particle.pos.col), i) == std::end(array)){
                int temp_len = skip_missing(column(M_copy, i)).size()
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

            int score = objective(M_copy, index);

            if (score >particle.best_value){
                particle.best_value = score;
                particle.updated = 0;
            }

            if (score > g_value){
                g_value = score;
                g = particle;
                g.best = getposition(particle.value, index_copy, M_copy).pos;
                g.best_value = score;
            }
        }
    }

    if (g_value == original_g_value){
        return NULL;
    }

    return &g;
}