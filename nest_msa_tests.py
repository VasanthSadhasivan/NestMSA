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
        sequences = ["a-bcbcdem", "acbcfg", "a-bchimn", "a-bcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.objective(matrix, 1), 2.625)

    def test_objective_2(self):
        sequences = ["abcbcdem", "a-cbcfg", "abc--himn", "abcbcjkm"]
        matrix = nest_msa.create_peer_matrix(sequences)
        self.assertEqual(nest_msa.objective(matrix, 3), 5.0)

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

    def test_criteria2_0(self):
        p = Particle(0, 0)
        p.updated = 3
        self.assertEqual(nest_msa.criteria2(p, 2), True)
      
    def test_criteria2_1(self):
        p = Particle(0, 0)
        p.updated = 3
        self.assertEqual(nest_msa.criteria2(p, 3), False)

    def test_criteria2_2(self):
        p = Particle(0, 0)
        p.updated = 3
        self.assertEqual(nest_msa.criteria2(p, 4), False)

    def test_criteria3_0(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = Particle('b', [1,[3]])
        self.assertEqual(nest_msa.criteria3(p, 2, M), True) 
   
    def test_criteria3_1(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = Particle('b', [1,[1, 2, 3]])
        self.assertEqual(nest_msa.criteria3(p, 2, M), False) 
     
    def test_criteria3_2(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = getPosition('d', 3, M)
        self.assertEqual(nest_msa.criteria3(p, 4, M), True) 

    def test_stopcriteria_0(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = Particle('b', [1,[1, 2, 3]])
        p.updated = 5
        self.assertEqual(nest_msa.stopcriteria(p, 2, M, 4), False)

    def test_stopcriteria_1(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = Particle('b', [1,[3]])
        p.updated = 4
        self.assertEqual(nest_msa.stopcriteria(p, 2, M), False)

    def test_stopcriteria_2(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'd', 'd'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        p = Particle('b', [1,[3]])
        p.updated = 6
        self.assertEqual(nest_msa.stopcriteria(p, 2, M), True)

    def test_row_alignment_0(self):
        M = [['a', 'a', 'b', 'c', 'd'],
            ['b', 'b', 'b', 'b', 'b'],
            ['b', 'c', 'c', 'd', 'd'],
            ['c', 'd', 'd', 'd', 'e']]
        self.assertEqual(nest_msa.row_alignment(1, M), [])

    def test_row_alignment_1(self):
        sequences = ["abcbcdem", "acbcfg", "abchimn", "abcbcjkm"])
        M = nest_msa.createPeerMatrix(sequences)
        correctp = Particle('c', [2, [2]])
        correctp.best = [3, [1]]
        correctp.best_value = 9.0
        self.assertEqual(nest_msa.row_alignment(1, M), correctp)

    def test_row_alignment_2(self):
        M = [['a', 'a', 'a', '-', 'a'],
            ['b', 'b', 'b', 'b', 'b'],
            ['c', 'c', 'c', 'c', 'c'],
            ['d', 'd', 'd', 'd', 'd']]
        self.assertEqual(nest_msa.row_alignment(0, M), [])
    
    def test_nest_msa_main(self):
        correctM = [['a', 'a', 'a', 'a'],
                    ['b', '-', 'b', 'b'],
                    ['c', 'c', 'c', 'c'],
                    ['b', 'b', '-', 'b'],
                    ['c', 'c', '-', 'c'],
                    ['d', 'f', 'h', 'j'],
                    ['e', 'g', 'i', 'k'],
                    ['m', '-', 'm', 'm'],
                    ['-', '-', 'n', '-']]
        M = createPeerMatrix(['abcbcdem', 'acbcfg', 'abchimn', 'abcbcjkm'])
        self.assertEqual(nest_msa.nest_msa_main(M), correctM)

if __name__ == '__main__':
    def test_stopcriteria_0(self):
    unittest.main()
