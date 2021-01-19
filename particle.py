class Particle():
    def __init__(self, c, pos):
        self.value = c
        self.updated = 0
        self.pos = pos #row_index, cols (row-index, [column_indeces])
        self.best = pos
        self.best_value = 0.0