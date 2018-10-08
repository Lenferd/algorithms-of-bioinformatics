import unittest
from pattern_count_problem import pattern_count

class PatternCountProblemTests(unittest.TestCase):
    def test_case_default(self):
        patter = "ATAT"
        genome = "GATATATGCATATACTT"
        expected = 3
        self.assertEqual(pattern_count(pattern=patter, str=genome), expected)

    def test_case_end(self):
        patter = "TTGL"
        genome = "ATGGGTTGLTGAATTGLGAGATCGACTTGLGTTAACTTGGGCTCAGGTTGL"
        expected = 4
        self.assertEqual(pattern_count(pattern=patter, str=genome), expected)

    def test_case_many_overlay(self):
        patter = "AAAA"
        genome = "AAAAAAAA"
        expected = 5
        self.assertEqual(pattern_count(pattern=patter, str=genome), expected)

if __name__ == '__main__':
    unittest.main()
