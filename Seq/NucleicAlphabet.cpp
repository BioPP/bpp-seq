/*
 * File ProteicAlphabet.cpp
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday July 22 2003
*/

#include "NucleicAlphabet.h" // class's header file

// class destructor
NucleicAlphabet::~NucleicAlphabet() {}

// Method to get the number of bases in alphabet
// return 4 : A, C, G, T (or U)
unsigned int NucleicAlphabet::getSize() const { return 4; }

// Method to get the number of char types in alphabet
// return 15 : gap isn't included, generic unresolved bases (N, X, ?, O, 0) count for one
unsigned int NucleicAlphabet::getNumberOfTypes() const { return 15; }
