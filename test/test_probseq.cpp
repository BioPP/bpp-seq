//
// File: test_probseq.cpp
// Created by: Murray Patterson
// Created on: Fri Oct 9 2015
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for numerical calculus. This file is part of the Bio++ project.

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

#include <iostream>
#include <string>
#include <vector>

#include <Bpp/Seq/Alphabet/BinaryAlphabet.h>
#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>

#include <Bpp/Seq/SymbolList.h>
#include <Bpp/Seq/ProbabilisticSymbolList.h>

#include <Bpp/Numeric/DataTable.h>

using namespace std;
using namespace bpp;

// convert a vector of string to a string
const string str(const vector<string> & v) {

  string s = "[";
  for(vector<string>::const_iterator i = v.begin(); i != v.end(); ++i)
    s += " " + *i;
  s += " ]";

  return s;
}

// basic info about an alphabet
void alphabet_info(const Alphabet * a) {

  cout << "alphabet is of size : " << a->getSize() << endl;
  cout << "supported chars : " << str(a->getSupportedChars()) << endl;
  cout << "resolved chars : " << str(a->getResolvedChars()) << endl;
}

// initialize empty (Probabilistic-) SymbolLists just to see
void init_empty_lists(const Alphabet * a) {
    BasicSymbolList list(a);
    BasicProbabilisticSymbolList p_list(a);
}
void init_empty_lists(const vector<Alphabet *> as) { // for several alphabets
  for(vector<Alphabet *>::const_iterator a = as.begin(); a != as.end(); ++a) { init_empty_lists(*a); }
}

/*
 * MAIN
 */
int main() {

  // initialize alphabets
  cout << endl << "init binary alphabet...";
  const BinaryAlphabet * a = new BinaryAlphabet();
  cout << "OK." << endl;
  alphabet_info(a);

  cout << endl << "init DNA alphabet...";
  const DNA * dna = new DNA();
  cout << "OK." << endl;
  alphabet_info(dna);

  // initialize empty (Probabilistic-) SymbolLists as a start
  cout << endl << "init (Probabilistic-) SymbolList with just alphabets...";
  init_empty_lists(a);
  init_empty_lists(dna);
  cout << "OK." << endl;

  /*
   * *** lists with binary content ***
   */
  cout << endl << "*** lists with binary content ***" << endl;

  // the normal version
  string c[] = {"1","0","0"};
  vector<string> content(c,c+sizeof(c)/sizeof(c[0]));

  cout << endl << "init binary list with : " << str(content) << " ...";
  BasicSymbolList list(content,a);
  cout << "OK." << endl;

  cout << "list contains : " << list.toString() << endl;
  
  // the probabilistic version
  istringstream iss("0 1\n0.85 0.15\n0.99 0.01");
  DataTable * data = DataTable::read(iss, " ", false);

  cout << endl << "init probabilistic version with : " << endl;
  DataTable::write(*data, cout);
  cout << "...";
  BasicProbabilisticSymbolList p_list(*data,a);
  cout << "OK." << endl;

  cout << "prob-list contains : ";
  DataTable::write(p_list.getContent(), cout);

  /*
   * *** lists with DNA content ***
   */
  cout << endl << "*** lists with DNA content ***" << endl;

  // the normal version
  string cc[] = {"G", "T", "C"};
  vector<string> dna_content(cc,cc+sizeof(cc)/sizeof(cc[0]));

  cout << endl << "init DNA list with : " << str(dna_content) << " ...";
  BasicSymbolList dna_list(dna_content,dna);
  cout << "OK." << endl;

  cout << "list contains : " << dna_list.toString() << endl;

  // the probabilistic version
  istringstream isss("0 0 1 0\n0.05 0 0.05 0.9\n0.01 0.97 0 0.02");
  DataTable * dna_data = DataTable::read(isss, " ", false);

  cout << endl << "init probabilistic version with : " << endl;
  DataTable::write(*dna_data, cout);
  cout << "...";
  BasicProbabilisticSymbolList dna_p_list(*dna_data,dna);
  cout << "OK." << endl;

  cout << "prob-list contains : ";
  DataTable::write(dna_p_list.getContent(), cout);

}