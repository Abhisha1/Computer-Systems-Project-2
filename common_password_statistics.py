from itertools import permutations
# common letter substitutions
common_substituions = {
    "a": ["A","@", "4"],
    "b": ["8", "B"],
    "c": ["C"],
    "d": ["D", "0"],
    "e": ["E", "3"],
    "f": ["F", "7"],
    "g": ["G", "6", "9", "&"],
    "h": ["H"],
    "i": ["I", "1", "l", "!"],
    "j": ["J"],
    "k": ["K"],
    "l": ["L", "1", "I"],
    "m": ["M"],
    "n": ["N", "V", "1"],
    "o": ["0", "O"],
    "p": ["P"],
    "q": ["Q", "0", "O"],
    "r": ["R"],
    "s": ["S", "5", "o", "$"],
    "t": ["T", "7"],
    "u": ["U", "(", ")"],
    "v": ["V"],
    "w": ["W"],
    "x": ["X"],
    "y": ["Y"],
    "z": ["Z", "5", "$"],
    "A": ["a","@", "4"],
    "B": ["8", "b"],
    "C": ["c"],
    "D": ["d", "0"],
    "E": ["e", "3"],
    "F": ["f", "7"],
    "G": ["g", "6", "9", "&"],
    "H": ["h"],
    "I": ["i", "1", "l", "!"],
    "J": ["j"],
    "K": ["k"],
    "L": ["l", "1", "I"],
    "M": ["m"],
    "N": ["n", "V", "1"],
    "O": ["0", "o"],
    "P": ["p"],
    "Q": ["q", "0", "O"],
    "R": ["r"],
    "S": ["s", "5", "o", "$"],
    "T": ["t", "7"],
    "U": ["u", "(", ")"],
    "V": ["v"],
    "W": ["w"],
    "X": ["x"],
    "Y": ["y"],
    "Z": ["z", "5", "$"]
}
def create_substituions(word):
    #creates a file of substitutions which is all possibly permutations
    f = open("substition.txt", "a")
    perms = [''.join(p) for p in permutations(word)]
    perms = list(dict.fromkeys(perms))
    for x in perms:
        f.write(x)
        f.write("\n")
    f.close()
    print(perms)
def create_replacements(word, file):
    # replaces world and writes to file
    for x in range(len(word)):
        if (word[x] in common_substituions):
            for y in common_substituions[word[x]]:
                new = word.replace(word[x], y)
                file.write(new)
                file.write("\n")

def create_common_password_substitutions():
    # creates list of passwords with common replacements
    f = open("test.txt", "r")
    words = f.readlines()
    replacements = open("replacements_test.txt", "a")
    for x in words:
        print(x)
        # create_substituions(x[:6])
        create_replacements(x[:6], replacements)
    replacements.close()
    f.close()

def get_character_frequencies():
    #gets the frequencies of characters and builds words
    f = open("proj-2_common_passwords.txt", "r")
    words = f.readlines()
    f.close()
    frequency = {}
    for x in words:
        for c in x:
            if (c != "\n" and c != " "):
                if (c in frequency.keys()):
                    frequency[c]+=1
                else:
                    frequency[c] = 1
    f = open("common_password_frequency.txt", "w")
    for x in sorted(frequency.items(), key=lambda item: item[1], reverse=True):
        f.write(x[0])
        f.write("\n")
    f.close()

# create_common_password_substitutions()
get_character_frequencies()

def write_out_common_substitutions():
    # writes out common substituion words
    f = open("common_subs.txt", "w")
    for char, subs in common_substituions.items():
        f.write(char)
        print(len(char))
        f.write(": ")
        for x in subs:
            f.write(x)
            f.write(" ")
        f.write("\n")
    f.close()

# write_out_common_substitutions()