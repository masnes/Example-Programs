A python script to generate random passphrases from a text file (see wordlist-basic.txt).

Usage:

    python passGen.py

generates passphrase using default parameters (4 words from the first 2000 most common words in the wordlist)

    python passGen.py [length]

  generates passphrase of [length] words

    python passGen.py [length] [number]

  generates passphrase of [length] words from the first [number] of words in the wordlist. Useful if you want to use fewer words but still maintain a high-entropy password.
