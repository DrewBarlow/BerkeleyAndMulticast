
PATH: str = "./run.txt"

def main() -> None:
    nums: list = []
    with open(PATH, 'r') as file:
        line = file.readline()

        while line:
            if "andomly initiali" in line:
                nums += [int(line.split()[-1])]

            line = file.readline()

    print(len(nums))
    print(sum(nums)//len(nums))

    return

if __name__ == "__main__":
    main()

