import unittest
import nest_msa
import particle

class NestMSATestCase(unittest.TestCase):
    def test_create_peer_matrix_0(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        correct_output = [['a', 'a', 'a', 'a'],
                          ['b', 'c', 'b', 'b'],
                          ['c', 'b', 'c', 'c'],
                          ['b', 'c', 'h', 'b'],
                          ['c', 'f', 'i', 'c'],
                          ['d', 'g', 'm', 'j'],
                          ['e', None, 'n', 'k'],
                          ['m', None, None, 'm']]
        self.assertEqual(nest_msa.create_peer_matrix(sequences), correct_output)

    def test_create_peer_matrix_1(self):
        sequences = ["abbc", "abcd", "bbcd", "cddd", "ddde"]
        correct_output = [['a', 'a', 'b', 'c', 'd'],
                          ['b', 'b', 'b', 'd', 'd'],
                          ['b', 'c', 'c', 'd', 'd'],
                          ['c', 'd', 'd', 'd', 'e']]
        self.assertEqual(nest_msa.create_peer_matrix(sequences), correct_output)

    def test_create_peer_matrix_2(self):
        sequences = ["abc", "", "def"]
        correct_output = [['a', None, 'd'],
                          ['b', None, 'e'],
                          ['c', None, 'f']]
        self.assertEqual(nest_msa.create_peer_matrix(sequences), correct_output)

    def test_create_peer_matrix_3(self):
        sequences = ["a-bcbcdem", "acbcfg", "a-bchimn", "a-bcbcjkm"]
        correct_output = [['a', 'a', 'a', 'a'],
                          ['-', 'c', '-', '-'],
                          ['b', 'b', 'b', 'b'],
                          ['c', 'c', 'c', 'c'],
                          ['b', 'f', 'h', 'b'],
                          ['c', 'g', 'i', 'c'],
                          ['d', None, 'm', 'j'],
                          ['e', None, 'n', 'k'],
                          ['m', None, None, 'm']]
        self.assertEqual(nest_msa.create_peer_matrix(sequences), correct_output)

    def test_weight_0(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.weight(matrix[0]), 1.0)

    def test_weight_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.weight(matrix[1]), 0.1875)

    def test_objective_0(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.objective(matrix, 1), 2.625)

    def test_objective_1(self):
        sequences = ["abcdef", "aaccee", "bbddff", "abaded", "abccdd", "abcdefg"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.objective(matrix, 1), 6.25)

    def test_objective_2(self):
        sequences = ["The quick brown fox jumped over the lazy dog", "The quick brn fox jumed oer te laxy dogg", "Thje quicc brownn foxy jnmped oevr the lazzy do"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertAlmostEqual(nest_msa.objective(matrix, 10), 11.0)

    def test_objective_3(self):
        sequences = ["The quick brown fox jumped over the lazy dog", "The quick brn fox jumed oer te laxy dogg", "Thje quicc brownn foxy jnmped oevr the lazzy do"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertAlmostEqual(nest_msa.objective(matrix, 34), 3.5)

    def test_mostfrequent_0(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.mostfrequent(matrix[1]), (3, 'b'))

    def test_mostfrequent_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.mostfrequent(matrix[2]), (3, 'c'))

    def test_mostfrequent_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.mostfrequent(matrix[3]), (2, 'b'))

    def test_full_row_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.full_row(matrix[0]), True)
    
    def test_full_row_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.full_row(matrix[1]), False)
    
    def test_full_row_3(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.full_row(matrix[2]), False)

    def test_full_row_4(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.full_row(matrix[3]), False)

    def test_remove_missing_rows_1(self):
        sequences = ["abcbcde", "acbcfg", "abchimn", "abcbcjkm"]
        matrix1 = nest_msa.create_peer_matrix(sequences)
        sequences = ["abcbcde", "acbcfg", "abchimn", "abcbcjkm"]
        matrix2 = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.remove_missing_rows(matrix1), matrix2)

    def test_remove_missing_rows_2(self):
        sequences = ["abcbcde", "acbcfg", "abchimn", "abcbcjkm-"]
        matrix1 = nest_msa.create_peer_matrix(sequences)
        sequences = ["abcbcde", "acbcfg", "abchimn", "abcbcjkm"]
        matrix2 = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.remove_missing_rows(matrix1), matrix2)

    def test_remove_missing_rows_3(self):
        sequences = ["abcbcde", "acbcfg", "abchimn---", "abcbcjkm--"]
        matrix1 = nest_msa.create_peer_matrix(sequences)
        sequences = ["abcbcde", "acbcfg", "abchimn-", "abcbcjkm"]
        matrix2 = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.remove_missing_rows(matrix1), matrix2)

    def test_get_position_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual((nest_msa.getposition('b', 2, matrix)).pos, (2, [1, 3, 4]))

    def test_get_position_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual((nest_msa.getposition('c', 2, matrix)).pos, (2, [2]))

    def test_get_position_3(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual((nest_msa.getposition('b', 1, matrix)).pos, (1, []))

    def test_fly_down_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        p = particle.Particle('b', (1, [0, 2, 3]))
        matrix_expected = nest_msa.create_peer_matrix(["a-bcbcdem", "acbcfg", "a-bchimn", "a-bcbcjkm"])
        self.assertEqual(nest_msa.fly_down(p, matrix), matrix_expected)

    def test_fly_down_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        p = particle.Particle('b', (1, [0, 2, 3]))
        matrix_expected = nest_msa.create_peer_matrix(["a---bcbcdem", "acbcfg", "a---bchimn", "a---bcbcjkm"])
        self.assertEqual(nest_msa.fly_down(p, matrix, stride=3), matrix_expected)

    def test_column_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.column(matrix, 0), ["a", "b", "c", "b", "c", "d", "e", "m"])

    def test_column_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.column(matrix, 1), ["a", "c", "b", "c", "f", "g", None, None])

    def test_aligned_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.aligned(matrix[0]), True)

    def test_aligned_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.aligned(matrix[7]), True)


    def test_aligned_3(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.aligned(matrix[6]), False)


    def test_create_swarm_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.create_swarm(0, matrix), [particle.Particle('a', (0, [0, 1, 2, 3]))])

    def test_create_swarm_2(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.create_swarm(1, matrix), [particle.Particle('b', (1, [0, 2, 3])), particle.Particle('c', (1, [1]))])

    def test_create_swarm_3(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.create_swarm(7, matrix), [particle.Particle('m', (7, [0, 3]))])


if __name__ == '__main__':
    unittest.main()
