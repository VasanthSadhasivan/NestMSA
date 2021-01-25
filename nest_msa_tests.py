import unittest
import nest_msa


class NestMSATestCase(unittest.TestCase):
    def test1(self):
        a = 'some'
        b = 'some'
        self.assertEqual(a, b)

    def test2(self):
        a = True
        b = True

        self.assertEqual(a, b)

if __name__ == '__main__':
    unittest.main()
