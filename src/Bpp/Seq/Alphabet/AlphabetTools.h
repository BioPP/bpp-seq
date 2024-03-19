// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABET_ALPHABETTOOLS_H
#define BPP_SEQ_ALPHABET_ALPHABETTOOLS_H

#include <Bpp/Numeric/VectorTools.h>
#include <typeinfo>

#include "BinaryAlphabet.h"
#include "IntegerAlphabet.h"
#include "CodonAlphabet.h"
#include "DNA.h"
#include "DefaultAlphabet.h"
#include "ProteicAlphabet.h"
#include "RNA.h"
#include "RNY.h"
#include "AllelicAlphabet.h"
#include "WordAlphabet.h"

// From the STL :
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief Utilitary functions dealing with alphabets.
 */
class AlphabetTools
{
public:
  static std::shared_ptr<const DNA> DNA_ALPHABET;
  static std::shared_ptr<const RNA> RNA_ALPHABET;
  static std::shared_ptr<const CodonAlphabet> DNA_CODON_ALPHABET;
  static std::shared_ptr<const CodonAlphabet> RNA_CODON_ALPHABET;
  static std::shared_ptr<const ProteicAlphabet> PROTEIN_ALPHABET;
  static std::shared_ptr<const DefaultAlphabet> DEFAULT_ALPHABET;

public:
  AlphabetTools() {}
  virtual ~AlphabetTools() {}

public:
  /**
   * @brief Character identification method for sequence's alphabet identification
   *
   * Return :
   * - -1 gap
   * - 1 DNA specific (no character!)
   * - 2 RNA specific (U)
   * - 3 Protein specific (characters E, F, I, L, P, Q)
   * - 4 Nucleotide specific (no character)
   * - 5 DNA or Protein specific (T)
   * - 6 RNA or Protein specific (no character)
   * - 7 Any alphabet (A, B, C, D, G, H, J, K, M, N, O, R, S, V, W, X, Y, Z, 0)
   * - 0 Unknown character
   *
   * @param state The character to test.
   * @return The type code.
   */
  static int getType(char state);

  /**
   * @brief This checks that all characters in the alphabet are coded by a string of same length.
   *
   * This method is used when states are coded by more than one character, typically: codon alphabets.
   *
   * @param alphabet The alphabet to check.
   * @return True if all text description have the same length (e.g. 3 for codon alphabet).
   */
  static bool checkAlphabetCodingSize(const Alphabet& alphabet);

  /**
   * @brief This checks that all characters in the alphabet are coded by a string of same length.
   *
   * This function performs the same work as the previous one, but deals with pointers
   * instead of reference. This may be more convenient since we often use pointers on alphabets.
   *
   * @param alphabet a pointer toward the alphabet to check.
   * @return True if all text description have the same length (e.g. 3 for codon alphabet).
   */
  static bool checkAlphabetCodingSize(const Alphabet* alphabet);

  /**
   * @brief In case that all states in the given alphabet have a string description of same length,
   * send this length; e.g. 3 for codon alphabets.
   *
   * @param alphabet The alphabet to analyse.
   * @return The common size of all text descriptionif there is one. Else throws an AlphabetException.
   */
  static unsigned int getAlphabetCodingSize(const Alphabet& alphabet);

  /**
   * @brief In case that all states in the given alphabet have a string description of same length,
   * send this length; e.g. 3 for codon alphabets.
   *
   * This function performs the same work as the previous one, but deals with pointers
   * instead of reference. This may be more convenient since we often use pointers on alphabets.
   *
   * @param alphabet a pointer toward the alphabet to analyse.
   * @return The common size of all text descriptionif there is one. Else throws an AlphabetException.
   */
  static unsigned int getAlphabetCodingSize(const Alphabet* alphabet);

  /**
   * @return True if the alphabet is an instanciation of the NucleicAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isNucleicAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<NucleicAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the DNA class.
   * @param alphabet The alphabet to check.
   */
  static bool isDNAAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<DNA>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the RNA class.
   * @param alphabet The alphabet to check.
   */
  static bool isRNAAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<RNA>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the ProteicAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isProteicAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<ProteicAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the Codon class.
   * @param alphabet The alphabet to check.
   */
  static bool isCodonAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<CodonAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the WordAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isWordAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<CoreWordAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the RNY class.
   * @param alphabet The alphabet to check.
   */
  static bool isRNYAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<RNY>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the BinaryAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isBinaryAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<BinaryAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the ProteicAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isIntegerAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<IntegerAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the DefaultAlphabet class.
   * @param alphabet The alphabet to check.
   */
  static bool isDefaultAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<DefaultAlphabet>(alphabet); }

  /**
   * @return True if the alphabet is an instanciation of the Allelic class.
   * @param alphabet The alphabet to check.
   */
  static bool isAllelicAlphabet(const Alphabet* alphabet) { return alphabetInheritsFrom<AllelicAlphabet>(alphabet); }

  /**
   * @brief Tell if two characters match according to a given alphabet.
   *
   * Example (DNA):
   * @verbatim
   * A,T: false
   * A,A: true
   * A,N: true
   * A,Y: false
   * N,Y: true
   * N,N: true
   * @endverbatim
   *
   * @return True if the two characters are identical, or are compatible if at least one of them is a generic character.
   * @param alphabet The alphabet to use.
   * @param i First character to check.
   * @param j Secondt character to check.
   */
  static bool match(std::shared_ptr<const Alphabet> alphabet, int i, int j)
  {
    std::vector<int> a = alphabet->getAlias(i);
    std::vector<int> b = alphabet->getAlias(j);
    std::vector<int> u = VectorTools::vectorIntersection(a, b);
    return u.size() > 0;
  }

private:
  template<class Y>
  static bool alphabetInheritsFrom(const Alphabet* alphabet)
  {
    try
    {
      const Y* t = dynamic_cast<const Y*>(alphabet);
      return t != nullptr;
    }
    catch (std::exception& e)
    {
      return false;
    }
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABET_ALPHABETTOOLS_H
