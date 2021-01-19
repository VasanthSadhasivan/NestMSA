import copy
from particle import  Particle
from collections import defaultdict

def full_row(row):
    return len(set(row)) == 1 and set(row) != {'-'}

def remove_missing_rows(M):
    M_new = []

    for i, row in enumerate(M):
        if set(row) != {'-'}:
            M_new.append(row)
    return M_new

def fly_down(particle, M, stride = 1):
    M = M + [['-' for i in M[0]] for j in range(stride)]

    for row_to_edit in range(len(M)-1, len(M)-1-stride ):
        for col_to_edit in range(len(M[0])):
            if col_to_edit in particle.pos[1]:
                M[row_to_edit][col_to_edit] = M[row_to_edit - stride][col_to_edit]

    return remove_missing_rows(M)

def column(matrix, i):
    return [row[i] for row in matrix]

def aligned(row):
    return len(set(row)) == 1

def create_swarm(index, M):
    char_to_particles = defaultdict(lambda c: Particle(c, (index, [])))

    for i, character in enumerate(M[index]):
        char_to_particles[character].pos[1].append(i)

    swarm = []

    for k, v in char_to_particles.items():
        swarm.append(v)

    return swarm

def row_alignment(index, M):
    row = M[index]

    if aligned(row):
        return []

    swarm = create_swarm(index, M)

    original_g = g = swarm[0]

    original_g_value = g_value = objective(M, index, endindex=index)

    for particle in swarm:
        index_copy = index
        M_copy = M.copy()

        particle.best_value = objective(M, index, endindex=index_copy)

        missing_p = set(range(len(M_copy[0]))) - set(p.pos.indexes)
        max_len = max([len(set(column(M, missinc_col)) - {'_'}) for missinc_col in missing_p])
        criteria_1 = max_len

        while not(stop_criteria(particle, index_copy, M_copy)) and index_copy < criteria_1:
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
                g.best = get_position(p.value, index_copy, M_copy)
                g.best_value = score

    if g_value == original_g_value:
        return []

    return g


def net_msa_main(M):
    for i in range(len(M)):
        globaly_optimal = row_alignment(i, M)
        if globaly_optimal:
            M = fly_down(globaly_optimal, M, globaly_optimal.best.row - globaly_optimal.pos.row)
    return M

