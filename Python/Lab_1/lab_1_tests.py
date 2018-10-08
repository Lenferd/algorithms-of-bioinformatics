import unittest
from pattern_count_problem import pattern_count
from frequent_words_problem import count_frequency
from reverse_complement_problem import reverse_complement

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


class FrequentWordsProblemTests(unittest.TestCase):
    def test_case_default(self):
        k = 4
        genome = "ACGTTGCATGTCGCATGATGCATGAGAGCT"
        expected = "CATG GCAT"
        self.assertCountEqual(count_frequency(text=genome, k=k), expected)

    def test_case_end(self):
        k = 4
        genome = "ATGGGTTGLTGAATTGLGAGATCGACTTGLGTTAACTTGGGCTCAGGTTGL"
        expected = "TTGL"
        self.assertCountEqual(count_frequency(text=genome, k=k), expected)

    def test_case_many_overlay(self):
        k = 4
        genome = "AAAAAAAABBBBABBBBABBBBB"
        expected = "AAAA"
        self.assertCountEqual(count_frequency(text=genome, k=k), expected)


class ReverseComplementProblem(unittest.TestCase):
    def test_case_default(self):
        input = "AAAACCCGGT"
        expected = "ACCGGGTTTT"
        self.assertEqual(reverse_complement(input), expected)

        
if __name__ == '__main__':
    unittest.main()
