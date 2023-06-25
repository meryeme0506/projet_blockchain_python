import pytest
import bip

def test_create_random_entropy():
    # Test de la génération d'entropie
    try:
        bip.create_random_entropy()
    except Exception:
        assert False, "Expected no exception when generating entropy"

def test_create_checksum():
    # Test de la génération de la somme de contrôle
    try:
        entropy = bip.create_random_entropy()
        bip.create_checksum(entropy)
    except Exception:
        assert False, "Expected no exception when generating checksum"

def test_convert_to_recovery_phrase():
    # Test de la conversion en phrase de récupération
    try:
        entropy = bip.create_random_entropy()
        checksum = bip.create_checksum(entropy)
        bip.convert_to_recovery_phrase(checksum)
    except Exception:
        assert False, "Expected no exception when converting to recovery phrase"

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

def test_create_recovery_phrase_from_entropy():
    # Test de la création de la phrase de récupération à partir d'une entropie donnée
    try:
        entropy = bip.create_random_entropy()
        bip.create_recovery_phrase_from_entropy(entropy)
    except Exception:
        assert False, "Expected no exception when creating recovery phrase from given entropy"

def test_extract_entropy():
    # Test de l'extraction de l'entropie à partir d'une phrase de récupération
    try:
        recovery_phrase = bip.create_recovery_phrase(12)
        bip.extract_entropy(recovery_phrase)
    except Exception:
        assert False, "Expected no exception when extracting entropy from recovery phrase"

def test_validate_recovery_phrase():
    # Test de la validation de la phrase de récupération
    try:
        recovery_phrase = bip.create_recovery_phrase(12)
        entropy = bip.extract_entropy(recovery_phrase)
        bip.validate_recovery_phrase(entropy, recovery_phrase)
    except Exception:
        assert False, "Expected no exception when validating recovery phrase"
