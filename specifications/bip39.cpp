#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <bitset>
#include <string>
#include <cctype>
#include <iostream>
// #include <pybind11/pybind11.h>
// #include "sha256/sha256/sha256.h"
#include "bip39.h"
#include "functions.h"

// namespace py = pybind11;

using namespace std;

vector<int> POSSIBLE_WORD_COUNT = {12, 15, 18, 21, 24};

string Bip39::create_random_entropy() {
	string random_bytes_hex = generate_random_bytes(this->entropy_length / 8);
	this->entropy_ = random_bytes_hex;
	return random_bytes_hex;
}

string Bip39::create_checksum(string& entropy_sequence) const {
	string result;

	string entropy_sha256 = sha256(hex_str_to_bin_str(entropy_sequence));
	string sha256_bin = hex_str_to_bin_str(entropy_sha256);

	string result_to_append = sha256_bin.substr(0, this->checksum);
	result = entropy_sequence + result_to_append;

	return result;
}

string Bip39::convert_to_recovery_phrase(string& checksum_sequence) const {
	vector<string> v;
	auto words_dict = get_words(this->language_);

	for (int i = 0; i < checksum_sequence.length(); i += 11) {
		auto sub = checksum_sequence.substr(i, 11);
		auto word = words_dict[stoi(sub, nullptr, 2)];
		v.push_back(word);
	}

	string result = accumulate(next(begin(v)), end(v), v[0], [](string a, string b) {
					return move(a) + " " + b;
				});

	return result;
}

string Bip39::create_recovery_phrase(int word_quantity) {
	if (find(begin(POSSIBLE_WORD_COUNT), end(POSSIBLE_WORD_COUNT), word_quantity) == end(POSSIBLE_WORD_COUNT)) {
		throw invalid_argument("The number of words wanted must be in [12, 15, 18, 21, 24] interval");
	}
	this->word_count = word_quantity;
	this->checksum = (this->word_count * 11) % 32;
	this->entropy_length = (this->word_count * 11) - this->checksum;

	auto entropy_sequence = this->create_random_entropy();
	entropy_sequence = hex_str_to_bin_str(entropy_sequence);
	auto checksum_sequence = this->create_checksum(entropy_sequence);
	this->mnemonic_  = this->convert_to_recovery_phrase(checksum_sequence);
	return this->mnemonic_;
}


//OK
string Bip39::extract_entropy(string recovery_phrase) {
	stringstream ss(recovery_phrase);
	string buf;
	vector<string> words;
	while (ss >> buf) {
		words.push_back(buf);
	}
	auto words_dict = get_words(this->language_);
	string words_string_binary;
	this->word_count = words.size();

	for_each(words.begin(), words.end(),
		[&](string const& w){
			auto it = find(words_dict.begin(), words_dict.end(), w);
			if (it == words_dict.end()) {
				throw invalid_argument("The word : " + w + "is not in the wordlist!");
			}
			words_string_binary += bitset<11>(it - words_dict.begin()).to_string();
		});

	string words_without_checksum = words_string_binary.substr(0, words_string_binary.length() - this->word_count);
	string result;

	for (auto i = 0; i < words_without_checksum.length(); i += 8){
        	result += bin_str_to_hex_str(words_without_checksum.substr(i, 8));
    	}
	this->entropy_ = result;
	return this->entropy_;
}


// // OK
// string Bip39::extract_entropy(string recoveryPhrase) {
// 	string entropy;
//
// // vérifie si une phrase secrète correspond à une séquence d'entropie donnée
// istringstream iss(recoveryPhrase);
// vector<string> words((istream_iterator<string>(iss)), istream_iterator<string>());
//
// 	for (auto &word : words) {
// 	size_t index = find(wordList.begin(), wordList.end(), word) - wordList.begin();
//
// 	// Debugging code
// 	if (index >= 2048) {
// 		cerr << "Error: 'index' is not less than 2048, it's " << index << "\n";
// 	}
//
// 	string bits = bitset<11>(index).to_string();
// 	entropy += bits;
// 	}
//
// return entropy;
// }
// OK
// string Bip39::create_checksum(string& entropy) const{
// 		// Calculer le hash SHA256 de l'entropie
// 		unsigned char hash[SHA256_DIGEST_LENGTH];
// 		SHA256(reinterpret_cast<const unsigned char*>(entropy.data()), entropy.size(), hash);
//
// 		// Récupérer les premiers bits du hash pour obtenir la somme de contrôle
// 		int checksumLength = entropy.size() / 4; // Nombre de bits de somme de contrôle (8 bits / 32 = 2 bits)
// 		string checksum;
// 		checksum.reserve(checksumLength);
//
// 		for (int i = 0; i < checksumLength; ++i) {
// 				checksum.push_back(hash[i / 8] >> (7 - (i % 8)) & 1 ? '1' : '0');
// 		}
//
// 		return checksum;
// }
// OK
// string Bip39::convert_to_recovery_phrase(string& entropyWithChecksum) const{
// string recoveryPhrase;
// recoveryPhrase.reserve(entropyWithChecksum.size() * 3 / 4); // Chaque groupe de 4 bits sera converti en un mot
//
// for (size_t i = 0; i < entropyWithChecksum.size(); i += 11) {
// 		string bits = entropyWithChecksum.substr(i, 11);
//
// if (bits.length() != 11) {
// 	cerr << "Error: 'bits' length is not 11, it's " << bits.length() << "\n";
// 	cerr << "entropyWithChecksum length: " << entropyWithChecksum.length() << ", i: " << i << "\n";
// }
// unsigned long long index = bitset<11>(bits).to_ullong();
//
// 		recoveryPhrase += wordList[index];
// 		recoveryPhrase.push_back(' ');
// }
//
// // Supprimer l'espace final
// recoveryPhrase.pop_back();
//
// return recoveryPhrase;
// }

string Bip39::create_recovery_phrase_from_entropy(string given_entropy) {
		string checksum = create_checksum(given_entropy);
		string entropyWithChecksum = given_entropy + checksum;

		return convert_to_recovery_phrase(entropyWithChecksum);
}

bool Bip39::validate_recovery_phrase(string& recoveryPhrase) {
		if (recoveryPhrase == "") {
			throw invalid_argument("empty phrase");
		} else {
			string extractedEntropy = extract_entropy(recoveryPhrase);
			string recreatedPhrase = create_recovery_phrase_from_entropy(extractedEntropy);

			if (recoveryPhrase == recreatedPhrase) {
				return recoveryPhrase == recreatedPhrase;
			}
			else{
				throw invalid_argument("Not a mnemonic phrase of a wallet");
			}
		}

}

// PYBIND11_MODULE(bip39, comp) {
// py::class_<BIP39>(m, "BIP39")
//     .def("create_recovery_phrase", &create_recovery_phrase)
// 	.def("validateRecoveryPhrase", &validateRecoveryPhrase);
//
// }
