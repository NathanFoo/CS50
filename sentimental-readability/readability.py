


# program that uses Coleman-Liau index to output reading
# a reading level of any given text by user.

text = input("Text: ")
letters = sentences = words = 0
newWord = True

for c in text:
    if c.isalpha():
        letters += 1
    if c == '.' or c == '!' or c == '?': # is end of sentence
        sentences += 1
    if newWord == True and c.isalpha():
        words += 1
        newWord = False
    elif newWord == False and c == ' ':
        newWord = True

print(f"Letters: {letters}")
print(f"Sentences: {sentences}")
print(f"Words: {words}")
l = (letters / words * 100)
s = (sentences / words * 100)
index = round(0.0588 * l - 0.296 * s - 15.8)

if index <= 0:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")

