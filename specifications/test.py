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
    # Test avec un bonne phrase mnémonique
    try:
        bip.validateRecoveryPhrase("A RENTRER")
    except Exception:
        assert False, "Expected no exception"

    # Test avec un phrases mnémonique erronées
    try:
        bip.validateRecoveryPhrase("    ")
    except ValueError:
        assert False, "Expected invalid_argument for empty mnemonic phrase"    

    try:
        bip.validateRecoveryPhrase("A RENTRER")
    except ValueError:
        pass
    else:
        assert False, "Expected invalid_argument for mnemonic phrase corresponding to no wallet" 