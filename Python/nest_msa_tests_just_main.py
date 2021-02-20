import unittest
import nest_msa


class NestMSATestCase(unittest.TestCase):
    def test_nest_msa_main_0(self):
        correct = [
            ['a', 'a', 'a', 'a'],
            ['b', '-', 'b', 'b'],
            ['c', 'c', 'c', 'c'],
            ['b', 'b', '-', 'b'],
            ['c', 'c', '-', 'c'],
            ['d', 'f', 'h', 'j'],
            ['e', 'g', 'i', 'k'],
            ['m', None, 'm', 'm'],
            [None, None, 'n', None]
            ]
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)

    def test_nest_msa_main_1(self):
        correct = [
            ['a', 'a', 'a', 'a', 'a'],
            ['-', '-', '-', 'a', 'a'],
            ['b', 'b', 'b', 'b', 'b'],
            ['-', 'c', 'c', 'c', 'c'],
            ['-', 'c', 'd', 'c', 'c'],
            ['-', 'd', 'd', 'd', 'c'],
            ['-', 'd', None, 'd', None],
            ['b', None, None, None, None],
            ['c', None, None, None, None],
            ['c', None, None, None, None],
            ['d', None, None, None, None],
            ['d', None, None, None, None]
            ]
        sequences = ["abbccdd", "abccdd", "abcdd", "aabccdd", "aabccc"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)

    def test_nest_msa_main_2(self):  # This one is not passing
        correct = [
            ['a', 'a', 'a', 'a', 'a', 'a'],
            ['b', 'b', 'b', 'b', 'b', '-'],
            ['c', 'c', 'c', 'd', 'c', 'd'],
            ['d', 'd', 'c', 'd', 'c', 'd'],
            ['d', None, None, None, None, None]
            ]
        sequences = ["abcdd", "abcd", "abcc", "abdd", "abcc", "add"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)  # This one is not passing

    def test_nest_msa_main_3(self):
        correct = [
            ['a', 'a'],
            ['b', 'b'],
            ['c', '-'],
            ['d', 'd'],
            ['d', 'd']
            ]
        sequences = ["abcdd", "abdd"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)
    
    def test_nest_msa_main_4(self):  # This one is not passing
        correct = [
            ['a', 'a'],
            ['a', 'a'],
            ['a', 'a'],
            ['b', '-'],
            ['b', '-'],
            ['b', '-'],
            ['c', 'c'],
            ['c', 'c'],
            ['c', 'c'],
            ['d', None],
            ['d', None],
            ['d', None]
            ]
        sequences = ["aaabbbcccddd", "aaaccc"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)  # This one is not passing

    def test_nest_msa_main_5(self):  # This one is not passing
        correct = [
            ['a', 'a'],
            ['b', 'b'],
            ['b', '-'],
            ['c', 'c'],
            ['d', 'd'],
            ['c', 'c'],
            ['b', 'b'],
            ['-', 'b'],
            ['a', 'a']
            ]
        sequences = ["abbcdcba", "abcdcbba"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.nest_msa_main(matrix), correct)  # This one is not passing


if __name__ == '__main__':
    unittest.main()
