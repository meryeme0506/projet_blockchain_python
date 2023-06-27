import bip39

def read_word_list_from_file(file_path):
    word_list = []
    with open(file_path, 'r') as file:
        for line in file:
            word_list.append(line.strip())
    return word_list

word_list = read_word_list_from_file("french.txt")
bip39 = bip39.BIP39Encoder(word_list)
word_quantity = 12

recovery_phrase = bip39.createRecoveryPhrase(word_quantity)
print("Recovery Phrase:", recovery_phrase)

entropy = bip39.extractEntropy(recovery_phrase)
print("Extracted Entropy:", entropy)

is_valid = bip39.validateRecoveryPhrase(recovery_phrase)
print("Is Valid Recovery Phrase:", is_valid)
