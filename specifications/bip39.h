#include <string>

class Bip39 {

    std::string recovery_phrase_;
    std::string language_;
    std::string entropy_;
    std::string mnemonic_;

    int checksum;
    int word_count;
    int entropy_length;

    std::string create_random_entropy();
    std::string create_checksum(std::string& entropy_sequence) const;
    std::string convert_to_recovery_phrase(std::string& checksum_sequence) const;

public:
    Bip39(std::string language = "en") : language_(language) {};
    ~Bip39() = default;

    std::string create_recovery_phrase(int word_quantity);
    std::string create_recovery_phrase_from_entropy(std::string given_entropy);
    std::string extract_entropy(std::string recovery_phrase);
    bool validate_recovery_phrase(std::string& recovery_phrase);

    std::string get_language() const { return this->language_; };
    void set_language(std::string language) { this->language_ = language; };
    std::string get_recovery_phrase() const { return this->recovery_phrase_; };
    std::string get_entropy() const {return this->entropy_; };
};
