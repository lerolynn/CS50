from cs50 import SQL
from sys import argv, exit
import csv


def main():
    if len(argv) != 2:
        print("Usage: python import.py characters.csv")
        exit(1)

    # Open sutdents database
    db = SQL("sqlite:///students.db")

    # Open file of character name and birth
    with open(argv[1], "r") as characters_file:
        characters = csv.reader(characters_file)

        # Insert student name, house, birthyear into database
        for student in characters:
            name = student[0].split(" ")

            # If student does not have a middle name, set middle name to zero
            if len(name) == 2:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                           name[0], None, name[1], student[1], student[2])

            elif len(name) == 3:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                           name[0], name[1], name[2], student[1], student[2])


if __name__ == "__main__":
    main()