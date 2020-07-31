from cs50 import get_int


def main():
    # Prompts user for card number
    while True:
        card_int = get_int("Number: ")
        if (card_int >= 0):
            break

    # Convert card number to string
    card = str(card_int)

    checksum = 0
    digit = 0

    # Apply Luhn's algorithm
    for i in range(len(card) - 1,  -1, 2):
        digit = int(card[i]) * 2
        if digit >= 10:
            checksum += 1
            checksum += digit - 10
        else:
            checksum += digit

    for i in range(len(card) - 1, -1, 2):
        checksum += int(card[i])

    # Check that number satisfies Luhn's algorithm
    if checksum % 10 != 0:
        print("INVALID")
        return 0

    # Check that card is AMEX
    if len(card) == 15 and int(card[0]) == 3 and int(card[1]) in [4, 7]:
        print("AMEX")

    # Check that card is MasterCard
    elif len(card) == 16 and int(card[0]) == 5 and int(card[1]) in range(1, 6):
        print("MASTERCARD")

    # Check that card is VISA
    elif len(card) in [13, 16] and int(card[0]) == 4:
        print("VISA")

    else:
        print("INVALID")

    return 0


if __name__ == "__main__":
    main()
