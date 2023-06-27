#include <string>

    using namespace std;

class Bip39 {
    

	string recovery_phrase_;
	string language_;
	string entropy_;
	string mnemonic;

	int checksum;
	int word_count;
	int entropy_length;

	/**
	 * @brief Crée une séquence d'entropie aléatoire
	 * @return Chaîne de caractères de l'entropie convertie en tampon binaire.
	 */
	string create_random_entropy();

	/**
     * @brief Génère une somme de contrôle et l'ajoute à l'entropie initiale
     * @param entropy_sequence Séquence d'entropie
	 * @return Chaîne de caractères de l'entropie avec la somme de contrôle ajoutée.
     */
	string create_checksum(string& entropy_sequence) const;
	
	/**
     * @brief Génère la phrase de récupération
	 * @param checksum_sequence Séquence d'entropie avec somme de contrôle
     * @return Chaîne de caractères de la phrase de récupération.
     */
	string convert_to_recovery_phrase(string& checksum_sequence) const;

public:
	Bip39(string language = "en") : language_(language) {};
	~Bip39() = default;

	/**
     * @brief Fonction principale pour générer une phrase de récupération
	 * @param word_quantity Nombre de mots, doit être dans [12, 15, 18, 21, 24]
     * @return Chaîne de caractères de la phrase de récupération dans la langue sélectionnée, par défaut "en";
     */
	string create_recovery_phrase(int word_quantity);

	/**
	 * @brief Génère une phrase de récupération à partir d'une entropie donnée
     * @param given_entropy Chaîne de caractères de l'entropie
     * @return Chaîne de caractères de la phrase de récupération dans la langue sélectionnée, par défaut "en";
	 */
	string create_recovery_phrase_from_entropy(string given_entropy);

	/**
	 * @brief Extrait l'entropie à partir d'une phrase de récupération
     * @param recovery_phrase Chaîne de caractères de la phrase de récupération
     * @return Chaîne de caractères de l'entropie
	 */
	string extract_entropy(string recovery_phrase);


	/**
	 * @brief Vérifie la phrase de récupération donnée par rapport à l'entropie.
	 * @param entropy_sequence Séquence d'entropie
	 * @param recovery_phrase Phrase de récupération
	 * @return Booléen indiquant si l'entropie donnée donne la même phrase de récupération
	 */
	bool validate_recovery_phrase(string& entropy_sequence, string& recovery_phrase);

	// Getters setters
	string get_language() const { return this->language_; };
	void set_language(string language) { this->language_ = language; };
	string get_recovery_phrase() const { return this->recovery_phrase_; };
	string get_entropy() const {return this->entropy_; };
};
