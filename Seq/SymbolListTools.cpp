/***************************************************************************
                          SymbolListTools.cpp  -  description
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

#include "SymbolListTools.h"

map<int, unsigned int> SymbolListTools::getCounts(const SymbolList & list) {
	map<int, unsigned int> c;
	vector<int> seq = list.getContent();
	for(unsigned int i = 0; i < seq.size(); i++) c[seq[i]]++;
	return c;
}

map<int, double> SymbolListTools::getFrequencies(const SymbolList & list) {
	map<int, double> f;
	vector<int> seq = list.getContent();
	for(unsigned int i = 0; i < seq.size(); i++) f[seq[i]]++;
	int n = seq.size();
	for(map<int, double>::iterator i = f.begin(); i != f.end(); i++) i -> second = i -> second / n;
	return f;
}
