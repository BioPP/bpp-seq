/*
 * File Sequence.h
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 21 2003
*/

// Secured inclusion of header's file
#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "SymbolList.h"
#include "SequenceExceptions.h"

// From the STL:
#include <string>
#include <vector>
using namespace std;

/**
 * @brief Declaration of Comments type.
 *
 * Commentaries are now defined only as a string vector to autorize later creation of
 * full Comments class.
 */
typedef vector<string> Comments;

/**
 * @brief The sequence object. This is a general purpose container, containing a ordered
 * list of states(= letters). The states that allowed to be present in the sequence are defined
 * by an alphabet object, which is passed to the sequence constructor by a pointer.
 *
 * For programming convenience, the states are stored as integers, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * Sequence objets also contain a name attribute and putativly some comments.
 *
 * @see Alphabet
 */
class Sequence: public SymbolList
{
	protected:

    /**
		 * @brief The sequence name.
		 */
		string _name;

		/**
		 * @brief The sequence comments.
		 */
		Comments _comments;

        /**
		 * @brief Sequence's sense (true : 5' -> 3'  /  false : 3' -> 5')
		 */
		mutable bool _sense;

    public: // Class constructors and destructor:

		/**
		 * @brief Direct constructor: build a Sequence object from a string
		 * You can use it safely for RNA, DNA and protein sequences.
		 *
		 * It can be used with codon sequences too, the string will be cut into
		 * parts of size 3. But for more complicated alphabets, you should use one
		 * complete constructors.
		 *
		 * @param name     The sequence name.
		 * @param sequence The whole sequence to be parsed as a string.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const string & sequence, const Alphabet * alpha) throw (BadCharException);
	
		/**
		 * @brief Direct constructor: build a Sequence object from a string
		 * You can use it safely for RNA, DNA and protein sequences.
		 *
		 * It can be used with codon sequences too, the string will be cut into
		 * parts of size 3. But for more complicated alphabets, you should use one
		 * complete constructors.
		 *
		 * @param name     The sequence name.
		 * @param sequence The whole sequence to be parsed as a string.
		 * @param comments Comments to add to the sequence.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const string & sequence, const Comments comments, const Alphabet * alpha) throw (BadCharException);
	
		/**
		 * @brief General purpose constructor, can be used with any alphabet.
		 *
		 * You should note that the sequence is stored as a vector of int.
		 * Hence each string in the vector will be translated using the alphabet object.
		 *
		 * @param name     The sequence name.
		 * @param sequence The sequence content.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const vector<string> & sequence, const Alphabet * alpha) throw (BadCharException);
		
		/**
		 * @brief General purpose constructor, can be used with any alphabet.
		 *
		 * You should note that the sequence is stored as a vector of int.
		 * Hence each string in the vector will be translated using the alphabet object.
		 *
		 * @param name     The sequence name.
		 * @param sequence The sequence content.
		 * @param comments Comments to add to the sequence.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const vector<string> & sequence, const Comments comments, const Alphabet * alpha) throw (BadCharException);
	
		/**
		 * @brief General purpose constructor, can be used with any alphabet.
		 *
		 * @param name     The sequence name.
		 * @param sequence The sequence content.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const vector<int> & sequence, const Alphabet * alpha) throw (BadIntException);
		
		/**
		 * @brief General purpose constructor, can be used with any alphabet.
		 *
		 * @param name     The sequence name.
		 * @param sequence The sequence content.
		 * @param comments Comments to add to the sequence.
		 * @param alpha    A pointer toward the alphabet to be used with this sequence.
		 */
		Sequence(const string & name, const vector<int> & sequence, const Comments comments, const Alphabet * alpha) throw (BadIntException);

		/**
		 * @brief The Sequence copy constructor. This does not perform a hard copy of the alphabet object.
		 */
		Sequence(const Sequence & s);
		
		/**
		 * @brief The Sequence copy operator. This does not perform a hard copy of the alphabet object.
		 *
		 * @return A ref toward the assigned Sequence.
		 */
		Sequence & operator = (const Sequence & s);

		~Sequence();

	public:
	
		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
		Clonable * clone() const;
		/** @} */
        
		
		/**
		 * @name Setting/getting the name of the sequence.
		 *
		 * @{
		 */
		 
		/**
		 * @brief Get the name of this sequence.
		 *
		 * @return This sequence name.
		 */
		const string getName() const;
		
		/**
		 * @brief Set the name of this sequence.
		 *
		 * @param name The new name of the sequence.
		 */
		void setName(const string & name);
		
		/** @} */
		
		/**
		 * @name Setting/getting the comments associated to the sequence.
		 *
		 * @{
		 */
		 
		/**
		 * @brief Get the comments associated to this sequence.
		 *
		 * @return The comments of the sequence.
		 */
		const Comments getComments() const;
		
		/**
		 * @brief Set the comments associated to this sequence.
		 *
		 * @param comments The new comments of the sequence.
		 */
		void setComments(const Comments & comments);
		
		/** @} */
		
		/**
		 * @name Setting/getting the sense of the sequence.
		 *
		 * @{
		 */
		 
		/**
		 * @brief Get the sense of the sequence.
		 *
		 * @return True is the sequence is 'sense', false if it is 'antisense'.
		 */
		bool getSense() const;
		
		/**
		 * @brief Set the sense of the sequence: 'true' if 'sense', 'false' if 'antisense'.
		 *
		 * @param sense The sense of the sequence.
		 */
		void setSense(bool sense);
		
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
		void setContent(const string & sequence) throw (BadCharException);

		/**
		 * @brief Set up the size of a sequence from the right side.
		 *
		 * All new characters are filled with gaps.
		 * If the specified size is < to the sequence size, the sequence will be truncated.
		 *
		 * @param size The new size of the sequence.
		 */
		void setToSizeR(unsigned int size);
		
		/**
		 * @brief Set up the size of a sequence from the left side.
		 *
		 * All new characters are filled with gaps.
		 * If the specified size is < to the sequence size, the sequence will be truncated.
		 *
		 * @param size The new size of the sequence.
		 */
		void setToSizeL(unsigned int size);

		/**
		 * @brief Append the specified content to the sequence.
		 *
		 * @param content The content to append to the sequence.
		 * @throw BadIntException If the content does not match the current alphabet.
		 */
		void append(const vector<int> & content) throw (BadIntException);

		/**
		 * @brief Append the specified content to the sequence.
		 *
		 * @param content The content to append to the sequence.
		 * @throw BadCharException If the content does not match the current alphabet.
		 */
		void append(const vector<string> & content) throw (BadCharException);

		/**
		 * @brief Append the specified content to the sequence.
		 *
		 * @param content The content to append to the sequence.
		 * @throw BadCharException If the content does not match the current alphabet.
		 */
		void append(const string & content) throw (BadCharException);

		/** @} */
};

#endif // _SEQUENCE_H_
