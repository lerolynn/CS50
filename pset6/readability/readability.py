from cs50 import get_string

# Get text from user
text = get_string("Text: ")

letters = 0
words = 0
sentences = 0

# Count number of words, letters, sentences in text
for i, j in enumerate(text):
    if (j.isalpha()):
        letters += 1

    if j.isspace() or i == len(text) - 1:
        words += 1

    if j in ['.', '!', '?']:
        sentences += 1

L = letters / words * 100
S = sentences / words * 100

# Coleman-Liau index
grade = round(0.0588 * L - 0.296 * S - 15.8)

# Print grade
if grade > 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade " + str(grade))