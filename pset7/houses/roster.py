from cs50 import SQL
from sys import argv, exit
import csv


def main():
    if len(argv) != 2:
        print("Usage: python roster.py House")
        exit(1)

    house = argv[1]

    # Open SQL database
    db = SQL("sqlite:///students.db")

    students = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last", house)

    for student in students:
        if student['middle'] == 'NULL' or student['middle'] == None:
            print(student['first'] + ' ' + student['last'] + ', born ' + str(student['birth']))
        else:
            print(student['first'] + ' ' + student['middle'] + ' ' + student['last'] + ', born ' + str(student['birth']))


if __name__ == "__main__":
    main()