import pytest
import bip39

# Créer une instance de Bip39
bip = bip39.Bip39()

# Générer une phrase mnémonique à partir d'une clé privée aléatoire
mnemonic = bip.create_recovery_phrase(12)
assert mnemonic, "La phrase mnémonique ne devrait pas être vide"

# Extraire l'entropie (qui est utilisée pour générer la clé privée) de la phrase mnémonique
entropy = bip.extract_entropy(mnemonic)
assert entropy, "L'entropie ne devrait pas être vide"

# Valider la phrase mnémonique en utilisant l'entropie
is_valid = bip.validate_recovery_phrase(entropy, mnemonic)
assert is_valid, "La phrase mnémonique devrait être valide"

# Afficher la clé (entropie)
print("La clé (entropie) est : ", entropy)
