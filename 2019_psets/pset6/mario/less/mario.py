#!/usr/bin/env python3

# Check for unacceptable inputs
while True:
    try:
        height = int(input("Height: "))
    except ValueError:
        continue
    if height < 1 or height > 8:
        continue
    else:
        break

# Loop through height and print pyramid
count = 0
while (count < height):
    count += 1
    print((height - count) * ' ', end="")
    print(count * '#')