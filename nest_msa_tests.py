import unittest
import nest_msa


class NestMSATestCase(unittest.TestCase):
    def test_create_peer_matrix_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        correct_output = [['a', 'a', 'a', 'a'],
                          ['b', 'c', 'b', 'b'],
                          ['c', 'b', 'c', 'c'],
                          ['b', 'c', 'h', 'b'],
                          ['c', 'f', 'i', 'c'],
                          ['d', 'g', 'm', 'j'],
                          ['e', None, 'n', 'k'],
                          ['m', None, None, 'm']]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(matrix, correct_output)

    def test_create_peer_matrix_2(self):
        sequences = ["abbc", "abcd", "bbcd", "cddd", "ddde"]
        correct_output = [['a', 'a', 'b', 'c', 'd'],
                          ['b', 'b', 'b', 'd', 'd'],
                          ['b', 'c', 'c', 'd', 'd'],
                          ['c', 'd', 'd', 'd', 'e']]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(matrix, correct_output)

    def test_create_peer_matrix_3(self):
        sequences = ["abc", "", "def"]
        correct_output = [['a', None, 'd'],
                          ['b', None, 'e'],
                          ['c', None, 'f']]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(matrix, correct_output)

    def test_create_peer_matrix_4(self):
        sequences = ["abc"]
        incorrect_output = [['']]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertNotEqual(matrix, incorrect_output)


if __name__ == '__main__':
    unittest.main()
