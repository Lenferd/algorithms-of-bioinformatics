DNA_COMPLEMENT = {"A":"T", "C":"G", "G":"C", "T":"A"}
def reverse_complement(input):
    input = input[::-1]
    return "".join([DNA_COMPLEMENT[k] for k in input])


if __name__ == '__main__':
    pattern = input()
    print(reverse_complement(pattern))
