#include <string>
#include <iostream>
#include <vector>
#include <openssl/sha.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <random>
#include <bitset>
#include <sstream>  
#include <algorithm>
#include <iterator>
#include "sha256.h"


namespace py = pybind11;

class BIP39Encoder {
private:
    std::vector<std::string> wordList; 

	// crée une séquence aléatoire de caractères qui 
	// servira d'entropie pour la création de la phrase secrète
    std::string createRandomEntropy() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        std::string entropy;
        entropy.reserve(32); // 256 bits / 8 = 32 bytes

        for (int i = 0; i < 32; ++i) {
            entropy.push_back(static_cast<char>(dis(gen)));
        }

        return entropy;
    }

	//crée une somme de contrôle à partir de la séquence d'entropie
	// 24 mots 264 bits  => clés 256 bits, dernier mot ==> incertitude, consulter le dic 
	//Fonction interne. Fonctionnement de l'interface clé 128 bits, 256 clés => 12 mots, clé 256 bits => 24 mots, dire comment générer les mots de contrôles 
	// la clé en HexaDécimal : 0X => convertir en l'enlevant, vérifier si la clé est une chaîne en HD, si contient un Z cas d'erreurs
    std::string createChecksum(std::string& entropy) {
        // Calculer le hash SHA256 de l'entropie
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(entropy.data()), entropy.size(), hash);

        // Récupérer les premiers bits du hash pour obtenir la somme de contrôle
        int checksumLength = entropy.size() / 4; // Nombre de bits de somme de contrôle (8 bits / 32 = 2 bits)
        std::string checksum;
        checksum.reserve(checksumLength);

        for (int i = 0; i < checksumLength; ++i) {
            checksum.push_back(hash[i / 8] >> (7 - (i % 8)) & 1 ? '1' : '0');
        }

        return checksum;
    }
	
	// transforme la séquence de caractères contenant l'entropie 
	// et la somme de contrôle en une phrase secrète
    std::string convertToRecoveryPhrase(std::string& entropyWithChecksum) {
    std::string recoveryPhrase;
	//
    recoveryPhrase.reserve(entropyWithChecksum.size() * 3 / 4); // Chaque groupe de 11 bits sera converti en un mot

    for (std::size_t i = 0; i < entropyWithChecksum.size(); i += 11) {
        std::string bits = entropyWithChecksum.substr(i, 11);

		if (bits.length() != 11) {
			std::cerr << "Error: 'bits' length is not 11, it's " << bits.length() << "\n";
			std::cerr << "entropyWithChecksum length: " << entropyWithChecksum.length() << ", i: " << i << "\n";
		}
		unsigned long index = std::bitset<11>(bits).to_ullong();

        recoveryPhrase += wordList[index];
        recoveryPhrase.push_back(' ');
    }

    // Supprimer l'espace final
    recoveryPhrase.pop_back();
	print(recoveryPhrase)
    return recoveryPhrase;
}


public:
    BIP39Encoder(std::vector<std::string> wordList) : wordList(wordList) {}

	// crée une phrase secrète avec un nombre spécifique de mots
    std::string createRecoveryPhrase(int wordQuantity) {
        std::string entropy = createRandomEntropy();
        std::string checksum = createChecksum(entropy);
        std::string entropyWithChecksum = entropy + checksum;

        return convertToRecoveryPhrase(entropyWithChecksum);
    }
	
	// crée une phrase secrète à partir d'une entropie donnée
    std::string createRecoveryPhraseFromEntropy(std::string givenEntropy) {
        std::string checksum = createChecksum(givenEntropy);
        std::string entropyWithChecksum = givenEntropy + checksum;

        return convertToRecoveryPhrase(entropyWithChecksum);
    }
	
	// extrait l'entropie à partir d'une phrase secrète
    std::string extractEntropy(std::string recoveryPhrase) {
    	std::string entropy;

    // vérifie si une phrase secrète correspond à une séquence d'entropie donnée
		std::istringstream iss(recoveryPhrase);
		std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    	for (auto &word : words) {
			std::size_t index = std::find(wordList.begin(), wordList.end(), word) - wordList.begin();

			// Debugging code
			if (index >= 2048) {
				std::cerr << "Error: 'index' is not less than 2048, it's " << index << "\n";
			}

			std::string bits = std::bitset<11>(index).to_string();
			entropy += bits;
    	}

    return entropy;
}


    bool validateRecoveryPhrase(std::string& recoveryPhrase) {
        std::string extractedEntropy = extractEntropy(recoveryPhrase);
        std::string recreatedPhrase = createRecoveryPhraseFromEntropy(extractedEntropy);

        return recoveryPhrase == recreatedPhrase;
    }
};

PYBIND11_MODULE(bip39, m) {
    m.doc() = "BIP39 Encoder/Decoder";

    py::class_<BIP39Encoder>(m, "BIP39Encoder")
        .def(py::init<std::vector<std::string>>())
        .def("createRecoveryPhrase", &BIP39Encoder::createRecoveryPhrase)
        .def("createRecoveryPhraseFromEntropy", &BIP39Encoder::createRecoveryPhraseFromEntropy)
        .def("extractEntropy", &BIP39Encoder::extractEntropy)
        .def("validateRecoveryPhrase", &BIP39Encoder::validateRecoveryPhrase);
}
