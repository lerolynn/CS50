from sys import argv, exit
import csv
import string


def main():

    # Check that CLI has correct number of arguments
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Open file with dna sequence
    sequence_file = open(argv[2], "r")
    dna = sequence_file.read()

    # Open csv file as database
    with open(argv[1], "r") as database:
        csv_list = list(csv.reader(database))

    # Get list of STRs profilled in database
    str_list = csv_list[0][1:]
    people_list = csv_list[1:]
    seq_count = [0] * len(str_list)
    max_count = [0] * len(str_list)
    # Count the number of consecutive STR of each sequence that appears in the dna sequence
    for i, seq in enumerate(str_list):
        seq_len = len(seq)
        # Loop through every element until STR is found
        for j in range(len(dna) - seq_len):
            start = j

            # Count number of consecutive STRs
            while(dna[start:start+seq_len] == seq):
                seq_count[i] += 1
                start += seq_len

            if max_count[i] < seq_count[i]:
                max_count[i] = seq_count[i]

            seq_count[i] = 0

    # Convert sequence count to string to compare with person in database
    max_count = [str(x) for x in max_count]
    person_found = False

    # Check if there is a match with a person
    for person in people_list:
        if max_count == person[1:]:
            print(person[0])
            person_found = True

    if person_found == False:
        print("No match")


if __name__ == "__main__":
    main()