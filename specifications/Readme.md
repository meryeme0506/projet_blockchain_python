****Auteurs**** : Meryeme HAMAN, Emilie PATHAMMAVONG et Alize BOUDIN

# Codeur/Decodeur BIP39 : Un Joyau Cryptographique pour les Portefeuilles de Cryptomonnaies

Ce document décrit la spécification d'un composant basé sur l'algorithme BIP39. Ce composant est destiné à être utilisé dans des applications nécessitant la création et la gestion de portefeuilles de cryptomonnaie codés en C++ et Python.

## Vue d'ensemble
Le composant BIP39 est un module de cryptographie, conçu pour générer des phrases de récupération pour les portefeuilles de cryptomonnaies. Il s'appuie sur le protocole BIP39 (Bitcoin Improvement Proposal 39), une pierre angulaire de la sécurité des cryptomonnaies.

## Description détaillée
BIP39 est un protocole qui permet de générer des phrases de récupération (aussi appelées "seed phrases" ou "mnemonic phrases") pour les portefeuilles de cryptomonnaie. Ces phrases sont généralement une liste de 12 à 24 mots qui sont utilisés pour récupérer l'accès à un portefeuille de cryptomonnaie.

Le composant BIP39 combine une clé secrète avec le message à hacher pour produire une phrase de récupération. Cette phrase peut ensuite être utilisée pour récupérer l'accès à un portefeuille de cryptomonnaie.

## Cas d'utilisation typiques
**Création de portefeuilles de cryptomonnaie** : BIP39 peut être utilisé pour générer une phrase de récupération lors de la création d'un nouveau portefeuille de cryptomonnaie.
**Récupération de portefeuilles de cryptomonnaie** : Si un utilisateur perd l'accès à son portefeuille de cryptomonnaie, il peut utiliser la phrase de récupération générée par BIP39 pour récupérer l'accès à son portefeuille.

## Schéma de blocs
[Application] --> [BIP39 Composant] --> [SHA256]
L'application interagit avec le composant BIP39, qui à son tour utilise le composant SHA256(Secure Hash Algorithm 256-bit).

## Interface et intéraction
L'interface du composant BIP39 est conçue pour être simple et directe. Elle fournit une fonction ****generate(word_count)**** qui prend un nombre de mots en entrée, et renvoie une phrase de récupération.

## Résumé des fonctions d'interface
1. `create_random_entropy()`: Cette fonction crée une séquence aléatoire de caractères qui servira d'entropie pour la création de la phrase secrète.

2. `create_checksum(std::string& entropy_sequence)`: Cette fonction crée une somme de contrôle à partir de la séquence d'entropie. Cette somme de contrôle est ajoutée à l'entropie pour assurer l'intégrité des données.

3. `convert_to_recovery_phrase(std::string& checksum_sequence)`: Cette fonction transforme la séquence de caractères contenant l'entropie et la somme de contrôle en une phrase secrète.

4. `create_recovery_phrase(int word_quantity)`: Cette fonction crée une phrase secrète avec un nombre spécifique de mots.

5. `create_recovery_phrase_from_entropy(std::string given_entropy)`: Cette fonction crée une phrase secrète à partir d'une entropie donnée.

6. `extract_entropy(std::string recovery_phrase)`: Cette fonction extrait l'entropie à partir d'une phrase secrète.

7. `validate_recovery_phrase(std::string& entropy_sequence, std::string& recovery_phrase)`: Cette fonction vérifie si une phrase secrète correspond à une séquence d'entropie donnée.

## Gestion des erreurs
1. `create_random_entropy()`: Une exception est déclenchée si la fonction échoue à produire une séquence d'entropie aléatoire.

2. `create_checksum(std::string& entropy_sequence)`: Une exception est déclenchée si la séquence d'entropie est vide ou si la création de la somme de contrôle échoue.

3. `convert_to_recovery_phrase(std::string& checksum_sequence)`: Une exception est déclenchée si la séquence de somme de contrôle est vide ou si la conversion de la séquence en phrase de récupération échoue.

4. `create_recovery_phrase(int word_quantity)`: Une exception est déclenchée si le nombre de mots spécifié n'est pas dans l'intervalle autorisé (par exemple, 12, 15, 18, 21, 24).

5. `create_recovery_phrase_from_entropy(std::string given_entropy)`: Une exception est déclenchée si l'entropie donnée est vide ou si la création de la phrase de récupération à partir de l'entropie échoue.

6. `extract_entropy(std::string recovery_phrase)`: Une exception est déclenchée si la phrase de récupération est vide ou si l'extraction de l'entropie à partir de la phrase de récupération échoue.

7. `validate_recovery_phrase(std::string& entropy_sequence, std::string& recovery_phrase)`: Une exception est déclenchée si la séquence d'entropie ou la phrase de récupération sont vides, ou si la phrase de récupération ne correspond pas à la séquence d'entropie.


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
        create_recovery_phrase(12)
    except Exception:
        assert False, "Expected no exception for valid word count"

    # Test avec un nombre de mots en dehors de l'intervalle [12, 15, 18, 21, 24]
    try:
        create_recovery_phrase(10)
    except ValueError:
        pass
    else:
        assert False, "Expected ValueError for word count outside of valid range"
```

Ce test vérifie que la fonction create_recovery_phrase(int word_quantity) crée une phrase de récupération avec le bon nombre de mots et qu'elle lève une exception ValueError lorsque le nombre de mots est en dehors de l'intervalle valide.

Des tests similaires peuvent être créés pour les autres fonctions.

### Exécution des tests
Pour exécuter les tests, il faudra simplement utiliser la commande python -m test test_bip39.