A python script to generate random passphrases from a text file (see wordlist-basic.txt) containing english words sorted by frequency of use.

Usage:

    python passGen.py

generates passphrase using default parameters (4 words from the first 2000 most common words in the wordlist)

    python passGen.py [length]

  generates passphrase of [length] words

    python passGen.py [length] [number]

  generates passphrase of [length] words from the first [number] of words in the wordlist. Useful if you want to use fewer words but still maintain a high-entropy password.


  Note: The wordfile contains over 250,000 words, far too much for me to check every word to see if they are appropriate or not. I've filtered out the obvious offenders, but there is some chance that something unsavory will appear in the passphrase.

  Disclaimer: Use at own risk. I am not responsible for your passwords.
