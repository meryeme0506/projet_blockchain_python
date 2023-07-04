import pytest
import bip39 as bip


def test_create_recovery_phrase():
    # Test avec un nombre de mots valide
    try:
        bip.create_recovery_phrase(12)
    except Exception:
        assert False, "Expected no exception for valid word count"

    # Test avec un nombre de mots en dehors de l'intervalle [12, 15, 18, 21, 24]
    try:
        bip.create_recovery_phrase(10)
    except ValueError:
        pass
    else:
        assert False, "Expected ValueError for word count outside of valid range"

def test_validateRecoveryPhrase():
    # Test avec un phrases mnémonique erronées
    try:
        bip.validate_recovery_phrase("")
    except ValueError:
        assert False, "Expected invalid_argument for empty mnemonic phrase"
    # Test avec un bonne phrase mnémonique
    try:
        bip.validate_recovery_phrase("legal winner thank year wave sausage worth useful legal winner thank yellow")
    except ValueError:
        pass
    else:
        assert False, "Expected no exception for valid mnemonic phrase"
