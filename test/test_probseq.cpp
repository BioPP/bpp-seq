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

// from the STL
#include <iostream>
#include <string>
#include <vector>

// alphabets
#include <Bpp/Seq/Alphabet/BinaryAlphabet.h>
#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>

// symbol lists
#include <Bpp/Seq/IntSymbolList.h>
#include <Bpp/Seq/ProbabilisticSymbolList.h>
#include <Bpp/Numeric/Table.h>

// sequences
#include <Bpp/Seq/Sequence.h>
#include <Bpp/Seq/ProbabilisticSequence.h>

// sites
#include <Bpp/Seq/Site.h>
#include <Bpp/Seq/ProbabilisticSite.h>

// containers
#include <Bpp/Seq/Container/VectorSiteContainer.h>
#include <Bpp/Seq/Container/VectorProbabilisticSiteContainer.h>

// file formats
#include <Bpp/Seq/Io/Fasta.h>
#include <Bpp/Seq/Io/Pasta.h>

using namespace std;
using namespace bpp;

typedef Table<double> DataTable;

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

  cerr << "alphabet is of size : " << a->getSize() << endl;
  cerr << "supported chars : " << str(a->getSupportedChars()) << endl;
  cerr << "resolved chars : " << str(a->getResolvedChars()) << endl;
}

// initialize empty (Probabilistic-) SymbolLists just to see
void init_empty_lists(const Alphabet * a) {
  BasicIntSymbolList list(a);
  BasicProbabilisticSymbolList p_list(a);
}
void init_empty_lists(const vector<Alphabet *> as) { // for several
  for(vector<Alphabet *>::const_iterator a = as.begin(); a != as.end(); ++a) { init_empty_lists(*a); }
}

/*
 * MAIN
 */
int main() {

  cerr << "========================================================" << endl;
      
  // initialize alphabets
  cerr << "init binary alphabet...";
  const BinaryAlphabet * a = new BinaryAlphabet();
  cerr << "OK." << endl;
  alphabet_info(a);

  cerr << endl << "init DNA alphabet...";
  const DNA * dna = new DNA();
  cerr << "OK." << endl;
  alphabet_info(dna);

  // initialize empty (Probabilistic-) SymbolLists as a start
  cerr << endl << "init (Probabilistic-) IntSymbolList with just alphabets...";
  init_empty_lists(a);
  init_empty_lists(dna);
  cerr << "OK." << endl;


  cerr << "========================================================" << endl;
  cerr << " LISTS " << endl;
  cerr << "========================================================" << endl;
  
  /*
   * *** lists with binary content ***
   */
  cerr << "*** lists with binary content ***" << endl;
  cerr << "***" << endl;

  // *** the normal version ***
  string c[] = {"1","0","0"};
  vector<string> content(c,c+sizeof(c)/sizeof(c[0]));

  cerr << endl << "init binary symbol list with : " << str(content) << " ...";
  BasicIntSymbolList list(content,a);
  cerr << "OK." << endl;

  cerr << "list contains : " << list.toString() << endl;

  // sequence
  cerr << endl << "init binary sequence with : " << str(content) << " ...";
  BasicSequence seq("basic binary sequence",content,a);
  cerr << "OK." << endl;

  // site
  cerr << endl << "init binary site with : " << str(content) << " and position 42...";
  Site site(content,a,42);
  cerr << "OK." << endl;

  cerr << "site has position : " << site.getPosition() << endl;
  
  // *** the probabilistic version ***
  istringstream iss("0 1\n0.85 0.15\n0.99 0.01");
  DataTable * data = DataTable::read(iss, false, " ", false);

  cerr << endl << "init probabilistic symbol list with : " << endl;
  
  DataTable::write(*data, cerr, false);
  cerr << "...";
  BasicProbabilisticSymbolList p_list(*data,a);
  cerr << "OK." << endl;

  cerr << "prob-list contains : " << endl;
  DataTable::write(p_list.getContent(), cerr, false);

  // sequence
  cerr << endl << "init binary probabilistic sequence with its content..." << endl;
  BasicProbabilisticSequence p_seq("basic probabilistic binary sequence",*data,a);
  cerr << "OK." << endl;

  // site
  cerr << endl << "init binary probabilistic site with its content and position 3..." << endl;
  ProbabilisticSite p_site(*data,a,3);
  cerr << "OK." << endl;

  cerr << "site has position : " << p_site.getPosition() << endl;
  
  /*
   * *** lists with DNA content ***
   */
  cerr << endl;
  cerr << "***" << endl;
  cerr << "*** lists with DNA content ***" << endl;
  cerr << "***" << endl;

  // *** the normal version ***
  string cc[] = {"G", "T", "C"};
  vector<string> dna_content(cc,cc+sizeof(cc)/sizeof(cc[0]));

  cerr << endl << "init DNA symbol list with : " << str(dna_content) << " ...";
  BasicIntSymbolList dna_list(dna_content,dna);
  cerr << "OK." << endl;

  cerr << "list contains : " << dna_list.toString() << endl;

  // sequence
  cerr << endl << "init DNA sequence with : " << str(dna_content) << " ...";
  BasicSequence dna_seq("basic DNA sequence",dna_content,dna);
  cerr << "OK." << endl;

  // site
  cerr << endl << "init DNA site with : " << str(dna_content) << " and position 23...";
  Site dna_site(dna_content,dna,23);
  cerr << "OK." << endl;

  cerr << "site has position : " << dna_site.getPosition() << endl;

  // *** the probabilist¡ic version ***
  istringstream isss("0 0 1 0\n0.05 0 0.05 0.9\n0.01 0.97 0 0.02");
  DataTable * dna_data = DataTable::read(isss, false, " ", false);

  cerr << endl << "init probabilistic DNA symbol list with : " << endl;
  DataTable::write(*dna_data, cerr, false);
  cerr << "...";
  BasicProbabilisticSymbolList dna_p_list(*dna_data,dna);
  cerr << "OK." << endl;

  cerr << "probabilistic list contains : " << endl;
  
  DataTable::write(dna_p_list.getTable(), cerr, false);


  // sequence
  cerr << endl << "init DNA probabilistic sequence with its content...";
  BasicProbabilisticSequence dna_p_seq("basic probabilistic binary sequence",*dna_data,dna);
  cerr << "OK." << endl;

  // site
  cerr << endl << "init DNA probabilistic site with its content...";
  ProbabilisticSite dna_p_site(*dna_data,dna,5);
  cerr << "OK." << endl;

  cerr << "site has position : " << dna_p_site.getPosition() << endl;

  
  cerr << "========================================================" << endl;
  cerr << "     CONTAINERS      " << endl;
  cerr << "========================================================" << endl;

  /*
   * *** vector (probabilistic) site containers ***
   */
  cerr << endl;
  cerr << "***" << endl;
  cerr << "*** vector site containers ***" << endl;
  cerr << "***" << endl;

  // *** the normal version ***
  cerr << endl << "Init vector site containers with just alphabets...";
  VectorSiteContainer container(a);
  VectorSiteContainer dna_container(dna);
  cerr << "OK." << endl;

  cerr << "=========================" << endl;
  cerr << "     BINARY " << endl;
  
  cerr << endl << "add binary sequence " << seq.toString() << " to binary container...";
  container.addSequence(seq);
  cerr << "OK." << endl;

  cerr << "binary container's first element is : ";
  cerr << container.getSequence(0).toString() << endl;

  // *** the probabilistic version ***
  cerr << endl << "Init vector probabilistic site containers with just alphabets...";
  VectorProbabilisticSiteContainer p_container(a);
  VectorProbabilisticSiteContainer dna_p_container(dna);
  cerr << "OK." << endl;

  cerr << endl << "add binary probabilistic sequence to binary probabilistic container...";

  p_container.addSequence(std::make_shared<BasicProbabilisticSequence>(p_seq));
  cerr << "OK." << endl;

  cerr << "binary probabilistic container's first element is : " << endl;
  DataTable::write(p_container.getSequence(0)->getTable(), cerr, false);

  /*
   * *** Fasta (and Pasta) files ***
   */
  cerr << endl;
  cerr << "***" << endl;
  cerr << "*** Fasta (and Pasta) files ***" << endl;
  cerr << "***" << endl;

  // *** the normal version ***
  Fasta * fasta = new Fasta();
  cerr << endl << "created a handler of type : " << fasta->getFormatName() << endl;

  string fasta_in = ">another binary sequence\n101\n";
  istringstream fasta_iss(fasta_in);
  cerr << "read the following into binary container..." << endl;
  cerr << endl << fasta_in << endl;
  fasta->appendSequencesFromStream(fasta_iss, container);
  cerr << "OK." << endl;
  
  cerr << "binary container contains : " << endl << endl;
  for(size_t i = 0; i < container.getNumberOfSequences(); ++i)
    cerr << container.getSequence(i).toString() << endl;
  cerr << endl;

  // *** the probabilistic version ***
  Pasta * pasta = new Pasta();
  cerr << "created a handler of type : " << pasta->getFormatName() << endl;

  string pasta_in = "0 1\n>a binary probabilistic sequence\n0.64 0.36\n0 1\n0.3 0.7\n";
  istringstream pasta_iss(pasta_in);
  cerr << "read the following into binary probabilistic container..." << endl;
  cerr << endl << pasta_in << endl;
  pasta->appendSequencesFromStream(pasta_iss, p_container);
  cerr << "OK." << endl;

  string pasta_in2 = ">another binary probabilistic sequence\n0.8 0.4 0.333\n";
  istringstream pasta_iss2(pasta_in2);
  cerr << "read the following into binary probabilistic container (in fast-track way for binary alphabets)..." << endl;
  cerr << endl << pasta_in2 << endl;
  pasta->appendSequencesFromStream(pasta_iss2, p_container);
  cerr << "OK." << endl;

  cerr << "binary probabilistic container contains : " << endl << endl;
  for(size_t i = 0; i < p_container.getNumberOfSequences(); ++i) {
    DataTable::write(p_container.getSequence(i)->getContent(), cerr, false);
    cerr << endl;
  }

  cerr << "binary probabilistic 1st site contains :" << endl << endl;
  cerr << p_container.getSite(0)->toString();
  cerr << endl << endl;

  cerr << "=========================" << endl;
  cerr << "     DNA " << endl;

  Fasta * dna_fasta = new Fasta();
  cerr << endl << "created a handler of type : " << fasta->getFormatName() << endl;
  string dna_fasta_in = ">a dna sequence\nACG\n";
  istringstream dna_fasta_iss(dna_fasta_in);
  cerr << "read the following into dna container..." << endl;
  cerr << endl << dna_fasta_in << endl;
  dna_fasta->appendSequencesFromStream(dna_fasta_iss, dna_container);
  cerr << "OK." << endl;

  cerr << "DNA container contains : " << endl << endl;
  for(size_t i = 0; i < dna_container.getNumberOfSequences(); ++i)
    cerr << dna_container.getSequence(i).toString() << endl;
  cerr << endl;

  // DNA...
  Pasta * dna_pasta = new Pasta();
  cerr << "created a handler of type : " << pasta->getFormatName() << endl;

  string dna_pasta_in = "A C G T\n>a dna prob. sequence\n0.1834088 0.6140376 0.132227880 0.07032571\n0.4960896 0.0523049 0.123549944 0.32805560\n";
  istringstream dna_pasta_iss(dna_pasta_in);
  cerr << "read the following into dna prob. container" << endl;
  cerr << endl << dna_pasta_in << endl;
  dna_pasta->appendSequencesFromStream(dna_pasta_iss, dna_p_container);
  cerr << "OK." << endl;

  string dna_pasta_in2 = "C T A G\n>another dna prob. sequence\n0.1885256 0.2023275 0.570924031 0.03822292\n0.1122945 0.2366416 0.004093129 0.64697079";
  istringstream dna_pasta_iss2(dna_pasta_in2);
  cerr << "read the following (permuted) sequence into dna prob. container" << endl;
  cerr << endl << dna_pasta_in2 << endl;
  dna_pasta->appendSequencesFromStream(dna_pasta_iss2, dna_p_container);
  cerr << "OK." << endl;

  cerr << "dna prob. container contains : " << endl << endl;
  for(size_t i = 0; i < dna_p_container.getNumberOfSequences(); ++i) {
    DataTable::write(dna_p_container.getSequence(i)->getTable(), cerr, false);
    cerr << endl;
  }

  

  cerr << "String of the 1st site:" << endl;
  
  cerr << dna_p_container.getSite(0)->toString() << endl << endl;
  cerr << "*******************************" << endl << endl;
  
  cerr << "DNA PASTA OUTPUT: " << endl << endl;
  cerr << "Regular container: " << endl << endl;

  dna_pasta->writeAlignedValues(cerr, dna_container);

  cerr << endl << "Probabilistic container: " << endl << endl;

  dna_pasta->writeSequences(cerr, dna_p_container);

  // the end


  return 0;
}
