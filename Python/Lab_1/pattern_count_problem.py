def pattern_count(pattern, str):
    occurance = 0
    p_len = len(pattern)
    for i in range(len(str) - p_len + 1):
        if (str[i:i+p_len] == pattern):
            occurance += 1
    return occurance

if __name__ == '__main__':
    pattern = input()
    genome = input()
    print(pattern_count(pattern, genome))
