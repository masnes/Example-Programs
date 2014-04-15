# This program is designed to read a text file of common passwords
# and subsequently build a passphrase to user specifications
# It can be run as is (python passGen.py), or
# with optional arguments [number of words], [selection from wordlist]

# ex: python passGen.py 3 20000
# creates a passphrase of 3 words, randomly selecting from the
# first 20,000 words on the wordlist

# This code written by Michael Asnes
import sys
import random


def setRandom():
    try:
        rnd = random.SystemRandom
    except AttributeError:
        sys.stderr.write("WARNING: Your system does not currently support "
                         "cryptographically secure random number generation. "
                         "Generated passwords may not be secure.\n"
                         "If you are using python version 2.4 or earlier "
                         "you may be able to fix this by updating to a newer "
                         "version of python.")
        rnd = random.Random
    return rnd


def passphrase(wordRange, numWords):
    rnd = setRandom
    rnd.seed()
    f = open("wordlist-basic.txt")
    lines = f.readlines()
    passphrase = ""
    ratingforWords = 1
    ratingforCharacters = 1

    for i in range(numWords):
        ratingforWords *= wordRange
        num = rnd.randint(1, wordRange)
        passphrase += lines[num].strip()
        if i < numWords:
            passphrase += " "

    for c in passphrase:
        if c is not " ":
            ratingforCharacters *= 26
    print "Passphrase:", passphrase

    entropyRatingforCharacters = ratingforCharacters.bit_length()
    entropyRatingforWords = ratingforWords.bit_length()

    if entropyRatingforCharacters < entropyRatingforWords:
        entropyRating = entropyRatingforCharacters
    else:
        entropyRating = entropyRatingforWords

    print "This passphrase has an entropy of", entropyRating, "bits"

    if entropyRating < 35:
        print "You probably want a password with an entropy of at least 35"
        print ("Increase the number of words, or select from a larger portion "
               "of words to increase the entropy")
    elif entropyRating < 45:
        print "An entropy rating of at least 35 is probably ok for most uses"
    else:
        print "An entropy rating of 45 or more is excellent!"


if len(sys.argv) == 2:
    numWords = int(sys.argv[1])
    passphrase(2000, numWords)
elif len(sys.argv) == 3:
    numWords = int(sys.argv[1])
    wordRange = int(sys.argv[2])
    if wordRange >= 245000:
        print ("Error, too large of a range, please enter a number less than "
               "245,000")
    elif wordRange < 1:
        print ("Error, too small of a range, please enter a number of at "
               "least 1")
    else:
        passphrase(wordRange, numWords)
else:
    passphrase(2000, 4)
