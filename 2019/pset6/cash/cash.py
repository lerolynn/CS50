#!/usr/bin/env python3
import cs50

# Check for input error
while True:
    try:
        dollars = float(input("Changed owed: "))
    except ValueError:
        continue
    if dollars < 0.0:
        continue
    else:
        break

cents = 100 * dollars
# Coins available: 25, 10, 5, 1
available_coins = [25, 10, 5, 1]
coin_count = 0
coin_type = 0

while cents > 0:
    coin_count += cents // available_coins[coin_type]
    cents = cents % available_coins[coin_type]
    coin_type += 1

print(int(coin_count))
