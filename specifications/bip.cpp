#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <bitset>
#include <string>
#include <pybind11/pybind11.h>
#include "sha256/sha256/sha256.h"

namespace py = pybind11;

#include "bip39.h"
#include "bip39_functions.h"

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
	auto words_dict = get_words(this->langage_);

	for (int i = 0; i < checksum_sequence.length(); i += 11) {
		auto sub = checksum_sequence.substr(i, 11);
		auto word = words_dict[stoi(sub, nullptr, 2)];
		v.push_back(word);
	}

	string result = accumulate(next(begin(v)), end(v), v[0],
				[](string a, string b) {
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

string Bip39::create_recovery_phrase_from_entropy(string given_entropy) {

	this->entropy_length = given_entropy.length() * 4;
	this->checksum = this->entropy_length / 32;
	this->word_count = (this->entropy_length + this->checksum) / 11;
	this->entropy_ = given_entropy;
	if (find(begin(POSSIBLE_WORD_COUNT), end(POSSIBLE_WORD_COUNT), this->word_count) == end(POSSIBLE_WORD_COUNT)) {
                throw invalid_argument("The entropy given doesn't have a word count in [12, 15, 18, 21, 24] interval");
        }

	auto entropy_sequence = hex_str_to_bin_str(given_entropy);
	auto checksum_sequence = this->create_checksum(entropy_sequence);
	this->mnemonic_ = this->convert_to_recovery_phrase(checksum_sequence);
	return this->mnemonic_;
}

string Bip39::extract_entropy(string recovery_phrase) {
	stringstream ss(recovery_phrase);
	string buf;
	vector<string> words;
	while (ss >> buf) {
		words.push_back(buf);
	}
	auto words_dict = get_words(this->langage_);
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

bool Bip39::validate_recovery_phrase(string& entropy_sequence, string& recovery_phrase) {

	int checksum = entropy_sequence.length() * 4 / 32;
	int word_quantity = (checksum + entropy_sequence.length() * 4) / 11;

	this->word_count = word_quantity;
        this->checksum = checksum;
        this->entropy_length = entropy_sequence.length() * 4;

	entropy_sequence = hex_str_to_bin_str(entropy_sequence);
        auto checksum_sequence = this->create_checksum(entropy_sequence);
        auto expected_recovery_phrase  = this->convert_to_recovery_phrase(checksum_sequence);
	return expected_recovery_phrase.compare(recovery_phrase) == 0;
}

PYBIND11_MODULE(bip39, comp) {
    py::class_<Bip39>(comp, "Bip39")
        .def(py::init<const string &>(), py::arg("langage") = "en")
	.def("get_langage", &Bip39::get_langage)
	.def("set_langage", &Bip39::set_langage)
	.def("get_recovery_phrase", &Bip39::get_recovery_phrase)
	.def("get_entropy", &Bip39::get_entropy)
        .def("create_recovery_phrase", &Bip39::create_recovery_phrase)
	.def("create_recovery_phrase_from_entropy", &Bip39::create_recovery_phrase_from_entropy)
	.def("extract_entropy", &Bip39::extract_entropy)
	.def("validate_recovery_phrase", &Bip39::validate_recovery_phrase);
}
