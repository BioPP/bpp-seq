//
// File: SequenceWithAnnotation.h
// Created by: Julien Dutheil
// Created on: Mon Jul 19 2010
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for sequences analysis.

  This software is governed by the CeCILL  license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability. 

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or 
  data to be ensured and,  more generally, to use and operate it in the 
  same conditions as regards security. 

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _SEQUENCEWITHANNOTATION_H_
#define _SEQUENCEWITHANNOTATION_H_

#include "CoreSequence.h"
#include "Sequence.h"
#include "IntSymbolList.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{

  class SequenceWithAnnotation;

/**
 * @brief Interface for sequence annotations.
 */
  class SequenceAnnotation :
    public virtual IntSymbolListListener
  {
  public:
    virtual SequenceAnnotation* clone() const = 0;

    /**
     * Creates a default annotation according to a given sequence.
     *
     * @param seq The template sequence (typically the one with which the annotation will be attached).
     */
    virtual void init(const Sequence& seq) = 0;

    /**
     * @return The type of the annotation. This is used for querying annotations.
     */
    virtual const std::string& getType() const = 0;

    /**
     * @brief Test is the annotation is valid for a given sequence.
     *
     * @param sequence The sequence to be validated against.
     * @param throwException If set to yes, throw an exception if the sequence is not valid.
     * @return true if this annotation is complient with the given sequence.
     */
    virtual bool isValidWith(const SequenceWithAnnotation& sequence, bool throwException = true) const = 0;

    /**
     * @brief Merge the input annotation with the current one.
     * 
     * @param anno The annotation to fuse.
     * @return true if the fusion was possible and succesful.
     */
    virtual bool merge(const SequenceAnnotation& anno) = 0;

    /**
     * @return A new annotation corresponding to a part of the sequence.
     * The implementation of this highly depends on the annotation type, and might not be supported.
     * @param pos Starting point of the region.
     * @param len The length of the region, in number of positions.
     */
    virtual SequenceAnnotation* getPartAnnotation(size_t pos, size_t len) const = 0;
};

/**
 * @brief An implementation of the Sequence interface that supports annotation. 
 *
 * This is a general purpose container, containing an ordered list of states.
 * The states that allowed to be present in the sequence are defined
 * by an alphabet object, which is passed to the sequence constructor by a pointer.
 *
 * For programming convenience, the states are stored as integers, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * Sequence objets also contain a name attribute and potentially several comment lines.
 *
 * The gestion of sequence content is identical to the BasicSequence object, but edition events are
 * properly fired. Listener are therefore properly handled.
 *
 * @see BasicSequence
 */
  class SequenceWithAnnotation :
    public virtual Sequence,
    public virtual AbstractCoreSequence,
    public virtual EdIntSymbolList
  {
  public:

    /**
     * @brief Empty constructor: build a void Sequence with just an Alphabet
     *
     * You can use it safely for all type of Alphabet in order to build an
     * empty Sequence i.e. without name nor sequence data.
     *
     * @param alpha    A pointer toward the Alphabet to be used with this Sequence.
     */
    SequenceWithAnnotation(const Alphabet* alpha);

    /**
     * @brief Direct constructor: build a Sequence object from a std::string
     * You can use it safely for RNA, DNA and protein sequences.
     *
     * It can be used with codon sequences too, the std::string will be cut into
     * parts of size 3. But for more complicated alphabets, you should use one
     * complete constructors.
     *
     * @param name     The sequence name.
     * @param sequence The whole sequence to be parsed as a std::string.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::string& sequence, const Alphabet* alpha);
  
    /**
     * @brief Direct constructor: build a Sequence object from a std::string.
     * 
     * You can use it safely for RNA, DNA and protein sequences.
     *
     * It can be used with codon sequences too, the std::string will be cut into
     * tokens of size 3. But for more complicated alphabets, you should use one
     * complete constructors.
     *
     * @param name     The sequence name.
     * @param sequence The whole sequence to be parsed as a std::string.
     * @param comments Comments to add to the sequence.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::string& sequence, const Comments& comments, const Alphabet* alpha);
  
    /**
     * @brief General purpose constructor, can be used with any alphabet.
     *
     * You should note that the sequence is stored as a std::vector of int.
     * Hence each std::string in the std::vector will be translated using the alphabet object.
     *
     * @param name     The sequence name.
     * @param sequence The sequence content.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::vector<std::string>& sequence, const Alphabet* alpha);
    
    /**
     * @brief General purpose constructor, can be used with any alphabet.
     *
     * You should note that the sequence is stored as a std::vector of int.
     * Hence each std::string in the std::vector will be translated using the alphabet object.
     *
     * @param name     The sequence name.
     * @param sequence The sequence content.
     * @param comments Comments to add to the sequence.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::vector<std::string>& sequence, const Comments& comments, const Alphabet* alpha);
  
    /**
     * @brief General purpose constructor, can be used with any alphabet.
     *
     * @param name     The sequence name.
     * @param sequence The sequence content.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::vector<int>& sequence, const Alphabet* alpha);
    
    /**
     * @brief General purpose constructor, can be used with any alphabet.
     *
     * @param name     The sequence name.
     * @param sequence The sequence content.
     * @param comments Comments to add to the sequence.
     * @param alpha    A pointer toward the alphabet to be used with this sequence.
     */
    SequenceWithAnnotation(const std::string& name, const std::vector<int>& sequence, const Comments& comments, const Alphabet* alpha);

    /**
     * @brief The Sequence generic copy constructor. This does not perform a hard copy of the alphabet object.
     */
  
    SequenceWithAnnotation(const Sequence& s);
   
    /**
     * @brief The Sequence copy constructor. This does not perform a hard copy of the alphabet object.
     */
    SequenceWithAnnotation(const SequenceWithAnnotation& s);
 
    /**
     * @brief The Sequence generic assignment operator. This does not perform a hard copy of the alphabet object.
     *
     * @return A ref toward the assigned Sequence.
     */
    SequenceWithAnnotation& operator=(const Sequence& s);
   
    /**
     * @brief The Sequence assignment operator. This does not perform a hard copy of the alphabet object.
     *
     * @return A ref toward the assigned Sequence.
     */
    SequenceWithAnnotation& operator=(const SequenceWithAnnotation& s);

    virtual ~SequenceWithAnnotation() {}

  public:
  
    /**
     * @name The Clonable interface
     *
     * @{
     */
    SequenceWithAnnotation* clone() const { return new SequenceWithAnnotation(*this); }
    /** @} */
        
    
    /**
     * @name Adjusting the size of the sequence.
     *
     * @{
     */
     
    /**
     * @brief Set the whole content of the sequence.
     *
     * @param sequence The new content of the sequence.
     * @see The Sequence constructor for information about the way sequences are internaly stored.
     */
    virtual void setContent(const std::string& sequence);

    void setContent(const std::vector<std::string>& list)
    {
      EdIntSymbolList::setContent(list);
    }

    void setContent(const std::vector<int>& list)
    {
      EdIntSymbolList::setContent(list);
    }

    std::string getChar(size_t pos) const
    {
      return EdIntSymbolList::getChar(pos);
    }

    void setToSizeR(size_t newSize);
    
    void setToSizeL(size_t newSize);

    /**
     * @brief Append the content of a sequence to the current one.
     *
     * @param seq The sequence to append. Only the raw content is appended, not additional fields such as annotations if any.
     * @throw AlphabetMismatchException In case the alphabet does not match the current one.
     */
    void append(const Sequence& seq);
    
    void append(const std::vector<int>& content);

    void append(const std::vector<std::string>& content);

    void append(const std::string& content);

    /** @} */

    /**
     * @brief Add a new annotation to the sequence.
     *
     * @param anno The annotation object to be added. Unless the annotation is shared,
     * the annotation object will be owned by the sequence object, and will be copied
     * and deleted when needed.
     * @throw Exception If the annotation is not valid for this sequence.
     * @see SequenceWithAnnotation::isValidWith
     */
    virtual void addAnnotation(SequenceAnnotation* anno)
    {
      anno->isValidWith(*this);
      addIntSymbolListListener(anno);
    }
 
    virtual bool hasAnnotation(const std::string& type) const
    {
      for (size_t i = 0; i < getNumberOfListeners(); ++i) {
        const IntSymbolListListener* listener = &getListener(i);
        const SequenceAnnotation* anno = dynamic_cast<const SequenceAnnotation*>(listener);
        if (anno && anno->getType() == type) return true;
      }
      return false;
    }

   
    virtual const SequenceAnnotation& getAnnotation(const std::string& type) const
    {
      for (size_t i = 0; i < getNumberOfListeners(); ++i) {
        const IntSymbolListListener* listener = &getListener(i);
        const SequenceAnnotation* anno = dynamic_cast<const SequenceAnnotation*>(listener);
        if (anno && anno->getType() == type) return *anno;
      }
      throw Exception("SequenceWithAnnotation::getAnnotation. No annotation found with type '" + type + "'.");
    }
    
    virtual SequenceAnnotation& getAnnotation(const std::string& type)
    {
      for (size_t i = 0; i < getNumberOfListeners(); ++i) {
        IntSymbolListListener* listener = &getListener(i);
        SequenceAnnotation* anno = dynamic_cast<SequenceAnnotation*>(listener);
        if (anno && anno->getType() == type) return *anno;
      }
      throw Exception("SequenceWithAnnotation::getAnnotation. No annotation found with type '" + type + "'.");
    }

    /**
     * @return The list of annotation types contained in this sequence.
     */
    virtual std::vector<std::string> getAnnotationTypes() const;

    /**
     * @brief Merge a sequence with the current one.
     *
     * Sequences must have the same name and alphabets.
     * Only first sequence's commentaries are kept.
     * Annotations that could not be merged will not be added in the concatenated sequence.
     * See the documentation of each annotation class for more details.
     *
     * @param swa The sequence to merge with.
     * @throw AlphabetMismatchException If the two alphabets do not match.
     * @throw Exception If the sequence names do not match.
     */
    virtual void merge(const SequenceWithAnnotation& swa);

    const Comments& getComments() const { return Commentable::getComments(); }

    void setComments(const Comments& comments) { Commentable::setComments(comments); }    

    void clearComments() { Commentable::clearComments(); }    

    const std::string& getName() const { return AbstractCoreSequence::getName(); }
    
    void setName(const std::string& name) { AbstractCoreSequence::setName(name); }
  };

} //end of namespace bpp.

#endif // _SEQUENCEWITHANNOTATION_H_

