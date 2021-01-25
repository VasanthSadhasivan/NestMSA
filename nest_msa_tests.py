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
