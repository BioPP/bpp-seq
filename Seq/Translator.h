//
// File: Translator.h
// Created by: jdutheil <Julien.Dutheil@univ-montp2.fr>
// Created on: Sun Oct 12 14:25:25 2003
//

#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include "Alphabet.h"
#include "Sequence.h"

/*
 * This interface is used when translating a sequence from an alphabet to another:
 * it gives the translation rules,
 * eg: RNA -> DNA.
 */
class Translator
{
	public:
		Translator();
		virtual ~Translator();
	
	public:
		virtual const Alphabet * getSourceAlphabet() const = 0;
		virtual const Alphabet * getTargetAlphabet() const = 0;
		virtual int    translate(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & c) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (Exception) = 0; 
};

/*
 * The same as previous, but can perform the reverse translation,
 * eg: RNA -> DNA and DNA -> RNA;
 */
class ReverseTranslator : public Translator
{
	public:
		ReverseTranslator();
		virtual ~ReverseTranslator();
	
	public:
		virtual const Alphabet * getSourceAlphabet() const = 0;
		virtual const Alphabet * getTargetAlphabet() const = 0;
		virtual int    translate(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & c) const throw (BadCharException, Exception) = 0;
		virtual int    reverse(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string reverse(const string & c) const throw (BadCharException, Exception) = 0;			
		virtual Sequence * reverse(const Sequence & sequence) const throw (Exception) = 0; 
};

class AbstractTranslator: public Translator
{
	public:
		AbstractTranslator();
		virtual ~AbstractTranslator();
	
	public:
		virtual const Alphabet * getSourceAlphabet() const = 0;
		virtual const Alphabet * getTargetAlphabet() const = 0;
		virtual int    translate(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & c) const throw (BadCharException, Exception) = 0;
		virtual Sequence * translate(const Sequence & sequence) const throw (Exception);
};

class AbstractReverseTranslator: public ReverseTranslator, public AbstractTranslator
{
	public:
		AbstractReverseTranslator();
		virtual ~AbstractReverseTranslator();
	
	public:
		virtual const Alphabet * getSourceAlphabet() const = 0;
		virtual const Alphabet * getTargetAlphabet() const = 0;
		virtual int    translate(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string translate(const string & c) const throw (BadCharException, Exception) = 0;
		virtual int    reverse(      int i     ) const throw (BadIntException, Exception)  = 0;		
		virtual string reverse(const string & c) const throw (BadCharException, Exception) = 0;			
		virtual Sequence * reverse(const Sequence & sequence) const throw (Exception);
};
#endif	//_TRANSLATOR_H_
