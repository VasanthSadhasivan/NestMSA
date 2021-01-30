import unittest
import nest_msa


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


if __name__ == '__main__':
    unittest.main()
