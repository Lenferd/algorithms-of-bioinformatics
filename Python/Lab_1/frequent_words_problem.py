def count_frequency(text, k):
    patterns = dict()
    t_len = len(text)
    for i in range(t_len - k + 1):
        if text[i: i + k] in patterns:
            patterns[text[i: i + k]] += 1
        else:
            patterns[text[i: i + k]] = 1
    max_val = max(patterns.values())
    return " ".join([k for k, v in patterns.items() if v == max_val])


if __name__ == '__main__':
    text = input()
    k = int(input())
    print(count_frequency(text, k))
