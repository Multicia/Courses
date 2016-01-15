def num_ones_in_binary(num):
    return sum([int(i) for i in str(bin(num)[2:]) if int(i) == 1])


def most_ones_in_binary(numbers):
    return sorted(numbers, key=num_ones_in_binary)[-1]

if __name__ == "__main__":
    print num_ones_in_binary(10)
    print most_ones_in_binary([0, 1, 2, 3, 5, 4, 2, 10])