#!/usr/bin/env python3

while True:
    try:
        # Get card number in string format
        card_number = int(input("Number: "))
    except ValueError:
        continue

    card_string = str(card_number)
    # Check Luhn's Algorithm
    luhn_sum = 0
    temp_digit = 0
    # Flip string card number around
    luhn_string = card_string[::-1]

    for i in range(len(luhn_string)):
        # For every other digit from 2nd to last digit of card_string
        if i % 2 == 1:
            temp_digit = 2 * int(luhn_string[i])
            luhn_sum += (temp_digit % 10) + (temp_digit // 10)

        else:
            luhn_sum += int(luhn_string[i])

    # Check that last digit is 0
    if luhn_sum % 10 == 0:
        # Check if AMEX: 15 digits, start with 34 or 37
        if len(card_string) == 15 and card_string[0] == '3' and (card_string[1] == '4' or card_string[1] == '7'):
            print('AMEX')
            break
        # Check if Mastercard: 16 digits, start with 51, 52, 53, 54, 55
        elif len(card_string) == 16 and card_string[0] == '5' and int(card_string[1]) in range(1, 6):
            print('MASTERCARD')
            break
        # Check if VISA: 13 and 16 digits
        elif (len(card_string) == 13 or len(card_string) == 16) and card_string[0] == '4':
            print('VISA')
            break

        else:
            print('INVALID')
            break
    # If not amex, mastercard or visa
    else:
        print('INVALID')
        break


# Output AMEX, MASTERCARD, VISA OR INVALID