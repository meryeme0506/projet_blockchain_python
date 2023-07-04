#include <string>
#include <sstream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits.h>

 
using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

std::string generate_random_bytes(int length) {
        random_bytes_engine rbe;
        rbe.seed(time(nullptr));
        std::vector<unsigned char> data(length);
        std::generate(begin(data), end(data), std::ref(rbe));
        std::string result{};
        std::for_each(begin(data), end(data),
                [&result](unsigned char c) {
                        std::stringstream stream;
                        stream << std::setfill('0') << std::setw(2) << std::hex << int(c);
                        result.append(stream.str());
                });

        return result;
}

const char* hex_char_to_bin(char c) {
	switch(toupper(c)) {
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default: throw std::invalid_argument("");
	}
}

std::string hex_str_to_bin_str(const std::string& hex) {
	std::string bin;
	for(unsigned i = 0; i != hex.length(); ++i) {
		bin += hex_char_to_bin(hex[i]);
	}
	return bin;
}


std::string bin_str_to_hex_str(const std::string& bin) {
    	const char *end = bin.c_str() + bin.length();
    	const char *cur = bin.c_str();
    	size_t x = 0;
    	while (cur != end && (*cur == '0' || *cur == '1')) {
        	x <<= 1;
        	x  |= *cur == '1';
        	cur++;
    	}
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(2) << std::hex << x;
    	return stream.str();
}


std::vector<std::string> get_words(std::string langage) {

	std::string file_name = "wordlists/";
	if (langage == "fr") {
		file_name += "french.txt";
	}
	else {
		file_name += "english.txt";
	}
	std::vector<std::string> words;

	std::ifstream input_stream(file_name);
	if (input_stream.is_open()) {
		std::string word;

		while (std::getline(input_stream, word)) {
			words.push_back(word);
		}
		input_stream.close();
	}
	else {

	}

	return words;
}