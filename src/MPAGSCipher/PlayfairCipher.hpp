#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

typedef std::pair<int, int> IntPair;
typedef std::map<char, std::pair<int, int>> Char2PairMap;
typedef std::map<std::pair<int, int>, char> Pair2CharMap;

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher, converting the given string into the key to be used by the cipher, by a call to setKey()
     * \param key the string to be converted into the cipher key
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief Create a key, converting the given string into the cipher key
     * \param key the string to convert into the key to be used in the cipher
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inTxt, const CipherMode cMode ) const;

  private:
    /// The cipher key, a phrase that will be used as a core of a 5*5 grid
    std::string key_{""};
    /// Lookup table to go from character to coordinates
    Char2PairMap ch2coord_;
    /// Lookup table to go from coordinates to the character
    Pair2CharMap coord2ch_;

};

#endif