/***************************************************************************
                          DCSE.h  -  description
                             -------------------
    begin                : mer mar 3 2004
    copyright            : (C) 2004 by Julien Dutheil
    email                : Julien.Dutheil@univ-montp2.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DCSE_H
#define DCSE_H

#include "AbstractISequence.h"
#include "Sequence.h"
#include "SequenceContainer.h"
#include "VectorSequenceContainer.h"

class DCSE : public AbstractISequence  {
    
	public: 
		DCSE() {};
		~DCSE() {};

	public:
	
		/**
		 * @name The ISequence interface
		 *
		 * @{
		 */
		VectorSequenceContainer * read(const string & path, const Alphabet * alpha) const throw (Exception) {
			return AbstractISequence::read(path, alpha);
		}
		void read(const string & path, VectorSequenceContainer & sc) const throw (Exception);
		/** @} */

		const string getFormatName() const;
		const string getFormatDescription() const;
};

#endif
