from particle import Particle
import copy
from collections import defaultdict

# Example
# python> print(create_peer_matrix(["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]))
# [['a', 'a', 'a', 'a'], ['b', 'c', 'b', 'b'], ['c', 'b', 'c', 'c'], ['b', 'c', 'h', 'b'], ['c', 'f', 'i', 'c'], ['d', 'g', 'm', 'j'], ['e', None, 'n', 'k'], ['m', None, None, 'm']]
def create_peer_matrix(list_of_strings):
    number_of_columns = len(list_of_strings)
    number_of_rows = max(len(string) for string in list_of_strings)
    matrix = [[None for c in range(number_of_columns)] for r in range(number_of_rows)]
    for column_index in range(number_of_columns):
        for row_index in range(len(list_of_strings[column_index])):
            matrix[row_index][column_index] = list_of_strings[column_index][row_index]
    return matrix


# Example
# python> pretty_print_matrix(create_peer_matrix(["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]))
# a a a a
# b c b b
# c b c c
# b c h b
# c f i c
# d g m j
# e - n k
# m - - m
def pretty_print_matrix(matrix):  # Note that this prints None as a '-'
    for row in matrix:
        for i in range(len(row)):
            if row[i] is None:
                print('-',end="")
            else:
                print(row[i], end="")
            if i != (len(row) - 1):
                print(" ", end="")
        print("")
    print("")


def weight(row, w1=0.25, w2=0.5, w3=1.0):
    if full_row(row):
        return w3

    row_length = len(row)
    max = mostfrequent(row)[0]

    if aligned(row):
        return w2 * max / row_length
    else:
        x = 0 if max <= 1 else max
        return w1 * x / row_length


def objective(M, row_index, end_index=-1):
    weights = float(sum([weight(row) for row in M[row_index:]]))
    C = mostfrequent(M[row_index])[0]
    A = sum([aligned(row) for row in M[row_index:]])

    end_index = len(M)-1 if end_index == -1 else end_index

    if end_index >= len(M):
        raise RuntimeError("End index exceeds matrix size")

    gaps = 0
    for row in M[row_index:end_index+1]:
        gaps += row.count('-')

    return weights * (A * C) / (1 + gaps)


def full_row(row):
    return len(set(row)) == 1 and (set(row) != {None} or set(row) != {'-'})


def remove_missing_rows(M):
    M_new = []
    for i, row in enumerate(M):
        skip = 1
        for each in row:
            if each is not None:
                skip = 0
                break
        if skip == 0:
            M_new.append(row)
    return M_new


def getposition(value, rowindex, matrix):
    indices = []
    row = matrix[rowindex]
    for i in range(len(row)):
        if row[i] == value:
            indices.append(i)
    return Particle(value,(rowindex, indices))

def mostfrequent(row):
    freqList = [[row.count(letter), letter] for letter in (set(row) - {'-'})]
    best = [0, None]
    for each in freqList:
        if each[0] > best[0]:
            best = each
    return (best[0], best[1])  


def fly_down(particle, M, stride = 1):
    M_new = copy.deepcopy(M) + [[None for _ in copy.deepcopy(M)[0]] for j in range(stride)]

    for row_to_edit in range(len(M_new)-1, particle.pos[0], -1):
        for col_to_edit in range(len(M_new[0])):
            if col_to_edit in particle.pos[1]:
                M_new[row_to_edit][col_to_edit] = M_new[row_to_edit - stride][col_to_edit]

    for col in particle.pos[1]:
        for row in range(stride):
            M_new[particle.pos[0] + row][col] = '-'
    return remove_missing_rows(M_new)


def column(matrix, i):
    return [row[i] for row in matrix]


def aligned(row):
    row_as_set = set(row)
    if (len(row_as_set) == 1) and (row_as_set != {None} or row_as_set != {'-'}):
        return True
    if (len(row_as_set) == 2) and (None in row_as_set or '-' in row_as_set):
        return True
    return False


def create_swarm(index, M):
    char_to_particles = {}

    for i, character in enumerate(M[index]):
        if character is None:
            continue
        if character not in char_to_particles:
            char_to_particles[character] = Particle(character, (index, []))
        char_to_particles[character].pos[1].append(i)

    swarm = []

    for k, v in char_to_particles.items():
        swarm.append(v)

    return swarm

def criteria3(p, newindex, M):
    return len(p.pos[1]) != len(getposition(p.value, newindex, M).pos[1])

def criteria2(p, threshold):
    return p.updated > threshold


def stopcriteria(p, newindex, M, threshold=5, debug=False):
    c2 = criteria2(p, threshold)
    c3 = criteria3(p, newindex, M)
    if (debug and c2):
        print("Terminating because of criteria 2")
    elif (debug and c3):
        print("Terminating because of criteria 3")
    return (c2 and c3)

def skip_missing(array):
    without_missing = []
    for elem in array:
        if elem is not None:
            without_missing.append(elem)
    return without_missing

def row_alignment(index, M):
    row = M[index]

    if aligned(row):
        return None

    swarm = create_swarm(index, M)

    original_g = g = swarm[0]

    original_g_value = g_value = objective(M, index, end_index=index)

    for particle in swarm:
        index_copy = index
        M_copy = copy.deepcopy(M)

        particle.best_value = objective(M, index, end_index=index_copy)

        missing_p = set(range(len(M_copy[0]))) - set(particle.pos[1])
        max_len = max([len(skip_missing(column(M_copy, missinc_col))) for missinc_col in missing_p])
        criteria_1 = max_len

        while index_copy < criteria_1-1 and not(stopcriteria(particle, index_copy, M_copy)):
            index_copy += 1
            particle.updated += 1

            M_copy = fly_down(particle, M_copy)

            score = objective(M_copy, index)

            if score > particle.best_value:
                particle.best_value = score
                particle.updated = 0

            if score > g_value:
                g_value = score
                g = copy.deepcopy(particle)
                g.best = getposition(particle.value, index_copy, M_copy).pos
                g.best_value = score

    if g_value == original_g_value:
        return None

    return g


def nest_msa_main(M):

    for i in range(len(M)):
        globaly_optimal = row_alignment(i, M)

        if globaly_optimal:
            M = fly_down(globaly_optimal, M, globaly_optimal.best[0] - globaly_optimal.pos[0])

    return M
