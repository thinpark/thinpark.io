import nltk

# Enter [all]
nltk.download()

nltk.pos_tag(nltk.word_tokenize("This is a good book."))
nltk.pos_tag(nltk.word_tokenize("Could you please book the flight?"))
