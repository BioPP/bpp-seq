//
// File: CodonAlphabet.h
// Created by: Julien Dutheil
// Created on: Sun Oct 12 17:41:56 2003
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for sequences analysis.

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided
  only with a limited warranty and the software's author, the holder of
  the economic rights, and the successive licensors have only limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards
  their requirements in conditions enabling the security of their
  systems and/or data to be ensured and, more generally, to use and
  operate it in the same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _CODONALPHABET_H_
#define _CODONALPHABET_H_

#include "NucleicAlphabet.h"
#include "WordAlphabet.h"
 
// From the STL:
#include <string>
#include <memory>

namespace bpp
{

/**
 * @brief Codon alphabet class.
 * @author Laurent Guéguen, Julien Dutheil
 * 
 * Since codons are made of 3 nucleic bases (RNA or DNA), this class
 * has a NucleicAlphabet field used to check char description. This
 * nucleic alphabet is passed to the constructor. This class also adds
 * some methods specific to codon manipulation.
 */
  
  class CodonAlphabet:
    public virtual CoreWordAlphabet,
    public AbstractAlphabet
  {
  protected:
    std::shared_ptr<const NucleicAlphabet> nAlph_;
  
  public: // Constructor and destructor.
		
    /**
     * @brief Builds a new codon alphabet from a nucleic alphabet.
     * 
     * @param alpha The nucleic alphabet to be used.
     */
    CodonAlphabet(const NucleicAlphabet* alpha) :
      AbstractAlphabet(),
      nAlph_(alpha)
    {
      build_();  
    }

    CodonAlphabet(const CodonAlphabet& bia) :
      AbstractAlphabet(bia),
      nAlph_(bia.nAlph_)
    {
    }

    CodonAlphabet& operator=(const CodonAlphabet& bia)
    {
      AbstractAlphabet::operator=(bia);
      nAlph_ = bia.nAlph_;
    
      return *this;
    }
  
    CodonAlphabet* clone() const
    {
      return new CodonAlphabet(*this);
    }

    virtual ~CodonAlphabet()
    {
    }
  
    std::string getAlphabetType() const
    {
      return "Codon(letter="+ nAlph_->getAlphabetType() + ")";
    }

  private:
    /**
     * @name Inner utilitary functions
     *
     * @{
     */
    bool containsUnresolved(const std::string& state) const;
    
    bool containsGap(const std::string& state) const;

    void build_();

    /** @} */

  public:

    /**
     * @name From AbstractAlphabet
     *
     * @{
     */
  
    unsigned int getNumberOfTypes() const {return 65;}
  
    unsigned int getSize() const 
    {
      return 64;
    }

    int getUnknownCharacterCode() const 
    {
      return 64;
    }

    bool isUnresolved(int state) const
    {
      return state>=64;
    }

    bool isUnresolved(const std::string& state) const
    {
      return isUnresolved(charToInt(state));
    }

    bool isResolvedIn(int state1, int state2) const;

    std::vector<int> getAlias(int state) const;

    std::vector<std::string> getAlias(const std::string& state) const;
    
    int getGeneric(const std::vector<int>& states) const
    {
      return states[0];
    }

    std::string getGeneric(const std::vector<std::string>& states) const
    {
      return states[0];
    }
    
    int charToInt(const std::string& state) const
    {
      if (state.size() != 3)
        throw BadCharException(state, "CodonAlphabet::charToInt", this);
      if (containsUnresolved(state))
        return static_cast<int>(getSize());
      if (containsGap(state))
        return -1;
      else return AbstractAlphabet::charToInt(state);
    }

    /**
     * @name Codon specific methods
     *
     * @{
     */

    /**
     * @brief Get the int code for a codon given the int code of the three underlying positions.
     *
     * The int code of each position must match the nucleic alphabet specified for this alphabet.
     * @param pos1 Int description for position 1.
     * @param pos2 Int description for position 2.
     * @param pos3 Int description for position 3.
     * @return The int code of the codon.
     */
    int getCodon(int pos1, int pos2, int pos3) const
    {
      return (nAlph_->isUnresolved(pos1)
              || nAlph_->isUnresolved(pos2)
              || nAlph_->isUnresolved(pos3))? getUnknownCharacterCode()
        : pos3 + 4*pos2 + 16 * pos1;
    }

    /**
     * @brief Get the char code for a codon given the char code of the
     * three underlying positions.
     *
     * The char code of each position must match the nucleic alphabet
     * specified for this alphabet.
     *
     * NB: This performs pos1 + pos2 + pos3 after checking for each
     * position validity.
     *
     * @param pos1 Char description for position 1.
     * @param pos2 Char description for position 2.
     * @param pos3 Char description for position 3.
     * @return The Char code of the codon.
     */

    std::string getCodon(const std::string& pos1, const std::string& pos2, const std::string& pos3) const
    {
      return pos1+pos2+pos3;
    }
  
    /**
     * @brief Get the int code of the first position of a codon given its int description.
     * 
     * @param codon The int description of the codon.
     * @return The int description of the first position of the codon.
     */
    
    int getFirstPosition(int codon) const
    {
      return isUnresolved(codon)?nAlph_->charToInt("N"):codon / 16;
    }
  
    /**
     * @brief Get the int code of the second position of a codon given its int description.
     * 
     * @param codon The int description of the codon.
     * @return The int description of the second position of the codon.
     */

    int getSecondPosition(int codon) const
    {
      return isUnresolved(codon)?nAlph_->charToInt("N"):(codon / 4) % 4;
    }
  
  
    /**
     * @brief Get the int code of the third position of a codon given its int description.
     * 
     * @param codon The int description of the codon.
     * @return The int description of the third position of the codon.
     */

    int getThirdPosition(int codon) const
    {
      return isUnresolved(codon)?nAlph_->charToInt("N"):codon % 4;
    }
  
    /**
     * @brief Get the char code of the first position of a codon given its char description.
     * 
     * @param codon The char description of the codon.
     * @return The char description of the first position of the codon.
     */

    std::string getFirstPosition (const std::string& codon) const
    {
      return codon.substr(0,1);
    }
  
  
    /**
     * @brief Get the char code of the second position of a codon given its char description.
     * 
     * @param codon The char description of the codon.
     * @return The char description of the second position of the codon.
     */
  
    std::string getSecondPosition(const std::string& codon) const
    {
      return codon.substr(1,1);
    }
  

    /**
     * @brief Get the char code of the third position of a codon given its char description.
     * 
     * @param codon The char description of the codon.
     * @return The char description of the third position of the codon.
     */

    std::string getThirdPosition(const std::string& codon) const
    {
      return codon.substr(2,1);
    }


    /**
     * @name From CoreWordAlphabet
     *
     * @{
     */
    
    unsigned int getLength() const 
    {
      return 3;
    }
    
    bool hasUniqueAlphabet() const 
    {
      return true;
    }

    const Alphabet* getNAlphabet(size_t n) const 
    {
      return nAlph_.get();
    }

    int getWord(const Sequence& seq, size_t pos = 0) const
    {
      if (seq.size() < pos + 3)
        throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, seq.size() - 3);
      return getCodon(seq[pos], seq[pos+1], seq[pos+2]);
    }

    /**
     * @brief Get the char code for a word given the char code of the
     * underlying positions.
     *
     * The char code of each position must match the corresponding alphabet specified at this position.
     * @param vpos vector description for all the positions.
     * @param pos the start position to match in the vector.
     * @return The string of the word.
     * @throw IndexOutOfBoundsException In case of wrong position.
     */
    std::string getWord(const std::vector<std::string>& vpos, size_t pos = 0) const
    {    
      if (vpos.size() < pos + 3)
        throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, vpos.size() - 3);

      return getCodon(vpos[pos], vpos[pos+1], vpos[pos+2]);
    }

    int getWord(const std::vector<int>& vpos, size_t pos = 0) const
    {    
      if (vpos.size() < pos + 3)
        throw IndexOutOfBoundsException("CodonAlphabet::getWord", pos, 0, vpos.size() - 3);

      return getCodon(vpos[pos], vpos[pos+1], vpos[pos+2]);
    }

    
    int getNPosition(int codon, size_t pos) const 
    {
      if (isUnresolved(codon))
        return nAlph_->getUnknownCharacterCode();
      else
        return (pos==0 ? codon/16:
                (pos==1? (codon/4)%4
                 : codon%4));
    }
    
    /**
     * @brief Get the int codes of each position of a word given its int description.
     *
     * @param word The int description of the word.
     * @return The int description of the positions of the codon.
     */

    std::vector<int> getPositions(int word) const
    {
      if (isUnresolved(word))
      {
        int n=nAlph_->getUnknownCharacterCode();
        return std::vector<int>{n,n,n};
      }
      else
        return std::vector<int>{word / 16, (word/4)%4, word%4};
    }


    /**
     * @brief Get the char code of the Nth position of a codon given its char description.
     * 
     * @param codon The char description of the codon.
     * @param pos the position in the codon (starting at 0)
     * @return The char description of the position of the codon.
     */

    std::string getNPosition(const std::string& codon, size_t pos) const
    {
      return codon.substr(pos,1);
    }

    /**
     * @brief Get the char codes of each position of a word given its char description.
     *
     * @param word The char description of the word.
     * @return The char description of the three positions of the word.
     */
    std::vector<std::string> getPositions(const std::string& word) const
    {
      return std::vector<std::string>{word.substr(0,1), word.substr(1,1), word.substr(2,1)};
    }

    /**
     * @brief Translate a whole sequence from letters alphabet to words alphabet.
     *
     * @param sequence A sequence in letters alphabet.
     * @param pos the start postion (default 0)
     * @return The corresponding sequence in words alphabet.
     * @throw AlphabetMismatchException If the sequence alphabet do not match the source alphabet.
     * @throw Exception                 Other kind of error, depending on the implementation.
     */
    Sequence* translate(const Sequence &sequence, size_t = 0) const;

    /**
     * @brief Translate a whole sequence from words alphabet to letters alphabet.
     *
     * @param sequence A sequence in words alphabet.
     * @return The corresponding sequence in letters alphabet.
     * @throw AlphabetMismatchException If the sequence alphabet do not match the target alphabet.
     * @throw Exception                 Other kind of error, depending on the implementation.
     */
    Sequence* reverse(const Sequence& sequence) const;

    /*
     *
     * @}
     */
    
    /**
     * @brief Get the number of G+C in codon
     * @param codon The int description of the codon.
     *
     * @return The number of G+C in codon
     */
    int getGCinCodon(int codon) const;

    /**
     * @return The nucleic alphabet associated to this codon alphabet.
     */
    
    const NucleicAlphabet* const getNucleicAlphabet() const
    {
      return nAlph_.get();
    }

    /**
     * @name Overloaded AbstractAlphabet methods.
     * @{
     */
    unsigned int getStateCodingSize() const { return 3; }
    /** @} */
  };

} //end of namespace bpp.

#endif	//_CODONALPHABET_H_

