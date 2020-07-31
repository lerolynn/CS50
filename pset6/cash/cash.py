from cs50 import get_float

# Prompt for change from the user
while True:
    dollars = get_float("Change owed: ")
    if dollars >= 0:
        break

cents = dollars * 100
coins = 0

# Add coins while subtracting cent value
while not cents == 0:
    if cents >= 25:
        cents -= 25
        coins += 1
    elif cents >= 10:
        cents -= 10
        coins += 1
    elif cents >= 5:
        cents -= 5
        coins += 1
    else:
        cents -= 1
        coins += 1

# Print out total number of coins
print(coins)

