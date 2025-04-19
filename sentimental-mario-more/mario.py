height = 0;
space = 2
while not height >= 1 or not height <= 8:
    try:
        height = int(input("Height: "))
    except ValueError:
        print("Input must be a number between 1 and 8")


for i in range(1, (height + 1)):
    print(" " * (height - i), end="")
    print("#" * i, end="")
    print(" " * space, end="")
    print("#" * i, end="")
    print()

