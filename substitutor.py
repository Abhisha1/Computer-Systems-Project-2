from itertools import permutations
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
    f = open("substition.txt", "a")
    perms = [''.join(p) for p in permutations(word)]
    perms = list(dict.fromkeys(perms))
    for x in perms:
        f.write(x)
        f.write("\n")
    f.close()
    print(perms)
def create_replacements(word, file):
    for x in range(len(word)):
        if (word[x] in common_substituions):
            for y in common_substituions[word[x]]:
                new = word.replace(word[x], y)
                file.write(new)
                file.write("\n")

def create_common_password_substitutions():
    f = open("test.txt", "r")
    words = f.readlines()
    replacements = open("replacements_test.txt", "a")
    for x in words:
        print(x)
        # create_substituions(x[:6])
        create_replacements(x[:6], replacements)
    replacements.close()
    f.close()



create_common_password_substitutions()