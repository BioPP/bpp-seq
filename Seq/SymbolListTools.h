/***************************************************************************
                          SymbolListTools.h  -  description
                             -------------------
    begin                : ven avr 9 2004
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

#ifndef SYMBOLLISTTOOLS_H
#define SYMBOLLISTTOOLS_H

#include "SymbolList.h"
// From the STL:
#include <map>
using namespace std;

class SymbolListTools {

	public: 
		~SymbolListTools() {}

	public:
		static map<int, unsigned int> getCounts(const SymbolList & list);
		static map<int, double> getFrequencies(const SymbolList & list);

};

#endif
