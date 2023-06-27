import pytest
import bip39 as bip

def test_create_random_entropy():
    # Test entropy generation
    b = bip.Bip39()
    try:
        entropy = b.create_random_entropy()
        print("Generated entropy: ", entropy)
    except Exception:
        assert False, "Expected no exception when generating entropy"

def test_create_recovery_phrase():
    # Test with a valid number of words
    b = bip.Bip39()
    try:
        recovery_phrase = b.create_recovery_phrase(12)
        print("Generated recovery phrase: ", recovery_phrase)
    except Exception:
        assert False, "Expected no exception for valid word count"

    # Test with a number of words outside the range [12, 15, 18, 21, 24]
    try:
        b.create_recovery_phrase(10)
    except ValueError:
        print("Correctly raised ValueError for word count outside of valid range")
    else:
        assert False, "Expected ValueError for word count outside of valid range"

def test_create_recovery_phrase_from_entropy():
    # Test creating recovery phrase from given entropy
    b = bip.Bip39()
    try:
        entropy = b.create_random_entropy()
        recovery_phrase = b.create_recovery_phrase_from_entropy(entropy)
        print("Generated recovery phrase from given entropy: ", recovery_phrase)
    except Exception:
        assert False, "Expected no exception when creating recovery phrase from given entropy"

def test_extract_entropy():
    # Test extracting entropy from recovery phrase
    b = bip.Bip39()
    try:
        recovery_phrase = b.create_recovery_phrase(12)
        extracted_entropy = b.extract_entropy(recovery_phrase)
        print("Extracted entropy from recovery phrase: ", extracted_entropy)
    except Exception:
        assert False, "Expected no exception when extracting entropy from recovery phrase"

def test_validate_recovery_phrase():
    # Test validating recovery phrase
    b = bip.Bip39()
    try:
        recovery_phrase = b.create_recovery_phrase(12)
        entropy = b.extract_entropy(recovery_phrase)
        is_valid = b.validate_recovery_phrase(entropy, recovery_phrase)
        print("Recovery phrase validation result: ", is_valid)
    except Exception:
        assert False, "Expected no exception when validating recovery phrase"
