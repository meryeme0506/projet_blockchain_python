#include <string>
#include <vector>
#include <openssl/sha.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class BIP39Encoder {
private:
    std::vector<std::string> wordList; 

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

    std::string convertToRecoveryPhrase(std::string& entropyWithChecksum) {
        std::string recoveryPhrase;
        recoveryPhrase.reserve(entropyWithChecksum.size() * 3 / 4); // Chaque groupe de 4 bits sera converti en un mot

        for (std::size_t i = 0; i < entropyWithChecksum.size(); i += 11) {
            std::string bits = entropyWithChecksum.substr(i, 11);
            std::size_t index = std::bitset<11>(bits).to_ulong();
            recoveryPhrase += wordList[index];
            recoveryPhrase.push_back(' ');
        }

        // Supprimer l'espace final
        recoveryPhrase.pop_back();

        return recoveryPhrase;    }

public:
    BIP39Encoder(std::vector<std::string> wordList) : wordList(wordList) {}

    std::string createRecoveryPhrase(int wordQuantity) {
        std::string entropy = createRandomEntropy();
        std::string checksum = createChecksum(entropy);
        std::string entropyWithChecksum = entropy + checksum;

        return convertToRecoveryPhrase(entropyWithChecksum);
    }

    std::string createRecoveryPhraseFromEntropy(std::string givenEntropy) {
        std::string checksum = createChecksum(givenEntropy);
        std::string entropyWithChecksum = givenEntropy + checksum;

        return convertToRecoveryPhrase(entropyWithChecksum);
    }

    std::string extractEntropy(std::string recoveryPhrase) {
        std::string entropy;
        entropy.reserve(recoveryPhrase.size() * 4 / 3); // Chaque mot est converti en un groupe de 4 bits

        // Supprimer les espaces
        recoveryPhrase.erase(std::remove(recoveryPhrase.begin(), recoveryPhrase.end(), ' '), recoveryPhrase.end());
        for (std::size_t i = 0; i < recoveryPhrase.size(); i += 4) {
            std::string word = recoveryPhrase.substr(i, 4);
            std::size_t index = std::find(wordList.begin(), wordList.end(), word) - wordList.begin();
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

PYBIND11_MODULE(bip39_component, m) {
    m.doc() = "BIP39 Encoder/Decoder";

    py::class_<BIP39Encoder>(m, "BIP39Encoder")
        .def(py::init<std::vector<std::string>>())
        .def("createRecoveryPhrase", &BIP39Encoder::createRecoveryPhrase)
        .def("createRecoveryPhraseFromEntropy", &BIP39Encoder::createRecoveryPhraseFromEntropy)
        .def("extractEntropy", &BIP39Encoder::extractEntropy)
        .def("validateRecoveryPhrase", &BIP39Encoder::validateRecoveryPhrase);
}
