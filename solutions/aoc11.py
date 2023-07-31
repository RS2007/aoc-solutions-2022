#!/usr/bin/python3.8
from enum import Enum



def simulate_monke_behaviour_part2(monkey_array, inspect_times_array):
    prod = 1
    for num in [monkey.divisible_by for monkey in monkey_array]:
        prod *= num
    for index, monkey in enumerate(monkey_array):
        copied_items = list(monkey.items)
        for item in copied_items:
            inspect_times_array[index] += 1
            arg1 = item if monkey.arg1 == "old" else monkey.arg1
            arg2 = item if monkey.arg2 == "old" else monkey.arg2
            worry_level = arg1 * arg2 if monkey.operation == Operation.MUL else arg1+arg2
            monkey.items.remove(item)
            worry_level %= prod
            if worry_level % monkey.divisible_by == 0:  # This modulo is expensive
                monkey_array[monkey.true_monkey].items.append(worry_level)
            else:
                monkey_array[monkey.false_monkey].items.append(worry_level)


def simulate_monke_behaviour_part1(monkey_array, inspect_times_array):
    for index, monkey in enumerate(monkey_array):
        copied_items = list(monkey.items)
        for item in copied_items:
            inspect_times_array[index] += 1
            arg1 = item if monkey.arg1 == "old" else monkey.arg1
            arg2 = item if monkey.arg2 == "old" else monkey.arg2
            worry_level = arg1 * arg2 if monkey.operation == Operation.MUL else arg1+arg2
            worry_level = worry_level // 3
            monkey.items.remove(item)
            if worry_level % monkey.divisible_by == 0:
                monkey_array[monkey.true_monkey].items.append(worry_level)
            else:
                monkey_array[monkey.false_monkey].items.append(worry_level)


class Operation(Enum):
    MUL = 1
    ADD = 2


class Monkey:
    def __init__(self):
        self.operation = None
        self.items = []
        self.arg1 = None
        self.arg2 = None
        self.divisible_by = None
        self.true_monkey = None
        self.false_monkey = None


def solve():
    input = open("../inputs/input11.txt").read()
    cleaned_input = list(map(lambda x: x.strip(), filter(
        lambda x: x != "", input.splitlines())))
    counter = 0
    monkey_array = []
    while counter < len(cleaned_input):
        counter += 1
        monkey = Monkey()
        monkey.items = (list(map(lambda x: int(
            x.strip()), cleaned_input[counter].split(":")[1].strip().split(","))))
        counter += 1
        [arg1, sign, arg2] = cleaned_input[counter].split("=")[
            1].strip().split(" ")
        monkey.arg1 = "old" if (arg1 == "old") else int(arg1)
        monkey.arg2 = "old" if (arg2 == "old") else int(arg2)
        if sign == "*":
            monkey.operation = Operation.MUL
        elif sign == "+":
            monkey.operation = Operation.ADD
        counter += 1
        monkey.divisible_by = int(cleaned_input[counter].split(" ")[-1])
        counter += 1
        monkey.true_monkey = int(cleaned_input[counter].split(" ")[-1])
        counter += 1
        monkey.false_monkey = int(cleaned_input[counter].split(" ")[-1])
        counter += 1
        monkey_array.append(monkey)
    inspect_times_array = [0 for _ in range(len(monkey_array))]
    # print("=======")
    for i in range(10000):
        simulate_monke_behaviour_part2(monkey_array, inspect_times_array)
    sorted_inspect_times = sorted(inspect_times_array, reverse=True)
    print(inspect_times_array)
    print(
       f" Answer: {sorted_inspect_times[0] * sorted_inspect_times[1]}")


if __name__ == "__main__":
    solve()
