version : 1.3

****Auteurs**** : Meryeme HAMAN, Emilie PATHAMMAVONG et Alize BOUDIN

# Codeur/Decodeur BIP39 : Un Joyau Cryptographique pour les Portefeuilles de Cryptomonnaies

Ce document décrit la spécification d'un composant basé sur l'algorithme BIP39.
Ce composant permet de se rappeler facilement d'une clé secrète de 128 à 256 bit à l'aide d'une liste de 12 à 24 mots français.
BIP39 ne génère donc pas la clé secrète mais la seed phrase à partir de laquelle on dérive la clé secrète.

## Vue d'ensemble
Le composant BIP39 est un module de cryptographie, conçu pour générer des phrases de récupération pour les portefeuilles de cryptomonnaies. Il s'appuie sur le protocole BIP39 (Bitcoin Improvement Proposal 39), une pierre angulaire de la sécurité des cryptomonnaies.

## Description détaillée
BIP39 est un protocole qui permet de générer des phrases de récupération (aussi appelées "seed phrases" ou "mnemonic phrases") pour les portefeuilles de cryptomonnaie. Ces phrases sont généralement une liste de 12 à 24 mots qui sont utilisés pour récupérer l'accès à un portefeuille de cryptomonnaie.

Le composant BIP39 combine une clé secrète avec le message à hacher pour produire une phrase de récupération.
Cette phrase peut ensuite être utilisée pour récupérer l'accès à un portefeuille de cryptomonnaie.

## Cas d'utilisation typiques
**Création de portefeuilles de cryptomonnaie** : BIP39 peut être utilisé pour générer une phrase de récupération lors de la
création d'un nouveau portefeuille de cryptomonnaie.
**Récupération de portefeuilles de cryptomonnaie** : Un utilisateur peut retrouver l'accès à son portefeuille en rentrant sa
phrase mnémonique s'il perd son wallet.

## Interface et intéraction
L'interface du composant BIP39 est conçue pour être simple et directe. Elle fournit 2 fonctions : une pour créer la phrase
mnémonique à la création du portefeuille, et une autre pour lire une phrase mnémonique en entrée et donner accès au portefeuille
correspondant.

## Résumé des fonctions d'interface

1. `create_mnemonic_phrase(word_quantity)`: Cette fonction crée une phrase secrète avec un nombre spécifique de mots.

2. `validate_recovery_phrase(recoveryPhrase)`: Cette fonction vérifie si
une phrase secrète correspond à un portefeuille.

## Gestion des erreurs

1. `create_mnemonic_phrase(word_quantity)`: Une exception est déclenchée si le nombre de mots spécifié n'est pas dans l'intervalle autorisé (par exemple, 12, 15, 18, 21, 24).

2. `validate_recovery_phrase(recoveryPhrase)`: Une exception est déclenchée si la phrase de récupération est vides,
ou si la phrase de récupération ne correspond pas à un portefeuille.

## Interface

L'interface correspond au fichier bip39.py


## Tests

### Plan de test
Nous testerons toutes les fonctions de l'interface de notre composant.
Pour chaque fonction, nous testerons différents cas, y compris des cas limites, et nous vérifierons que chaque fonction renvoie le résultat attendu et qu'elle lève les exceptions appropriées en cas d'erreur.

### Programme de test
Nous allons vérifier toutes les fonctions sur Python.
Voici un exemple de programme de test pour la fonction create_recovery_phrase(int word_quantity) :

```python
def test_create_recovery_phrase():
    # Test avec un nombre de mots valide
    try:
        create_mnemonic_phrase(12)
    except Exception:
        assert False, "Expected no exception for valid word count"

    # Test avec un nombre de mots en dehors de l'intervalle [12, 15, 18, 21, 24]
    try:
        create_mnemonic_phrase(10)
    except ValueError:
        pass
    else:
        assert False, "Expected ValueError for word count outside of valid range"

def test_validateRecoveryPhrase():
    # Test avec un phrases mnémonique erronées
    try:
        validate_recovery_phrase("")
    except Exception:
        assert False, "Expected no exception"

    # Test avec un bonne phrase mnémonique
    try:
          validate_recovery_phrase("legal winner thank year wave sausage worth useful legal winner thank yellow")
      except ValueError:
          pass
      else:
          assert False, "Expected no exception for valid mnemonic phrase"     
```

Ces tests vérifient que la fonction create_recovery_phrase(int word_quantity) crée une phrase de récupération avec le bon nombre de mots et qu'elle lève une exception ValueError lorsque le nombre de mots est en dehors de l'intervalle valide et qu'une phrase mnémonique valable est rentré pour validateRecoveryPhrase.

### Exécution des tests
Pour exécuter les tests, il faudra simplement utiliser la commande python -m test test_bip39.
