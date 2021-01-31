class Particle():
    def __init__(self, c, pos):
        self.value = c
        self.updated = 0
        self.pos = pos #row_index, cols (row-index, [column_indeces])
        self.best = pos
        self.best_value = 0.0

    def __repr__(self):
        return "<value={} updated={} pos={} best={} best_value={}>".format(self.value, self.updated, self.pos, self.best, self.best_value)

    def __eq__(self, other):
        return other.value == self.value and other.updated == self.updated and other.pos == self.pos and other.best == self.best and other.best_value == self.best_value