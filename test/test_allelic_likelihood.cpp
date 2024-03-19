// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// from the STL
#include <iostream>
#include <string>
#include <vector>

// alphabets

#include <Bpp/Seq/Alphabet/AllelicAlphabet.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>

#include <Bpp/Numeric/Table.h>

// sequences
#include <Bpp/Seq/Sequence.h>
#include <Bpp/Seq/ProbabilisticSequence.h>

// sites
#include <Bpp/Seq/Site.h>
#include <Bpp/Seq/ProbabilisticSite.h>

// containers
#include <Bpp/Seq/Container/VectorSiteContainer.h>

// file formats
#include <Bpp/Seq/Io/Fasta.h>
#include <Bpp/Seq/Io/Pasta.h>

using namespace std;
using namespace bpp;

typedef Table<double> DataTable;

// convert a vector of string to a string
const string str(const vector<string>& v)
{
  string s = "[";
  for (vector<string>::const_iterator i = v.begin(); i != v.end(); ++i)
  {
    s += " " + *i;
  }
  s += " ]";

  return s;
}

/*
 * MAIN
 */
int main()
{
  // From pasta
  string nameSeq = "counts.pa";
  Pasta pasta;
  auto allelicAlpha = make_shared<const AllelicAlphabet>(AlphabetTools::DNA_ALPHABET, 4);

  auto stateAlpha =  allelicAlpha->getStateAlphabet();
  auto alphaPtr = dynamic_pointer_cast<const Alphabet>(stateAlpha);
  auto sites = pasta.readAlignment(nameSeq, alphaPtr);

  auto alphaPtr2 = dynamic_pointer_cast<const Alphabet>(allelicAlpha);
  ProbabilisticVectorSiteContainer sites2(alphaPtr2);
  for (size_t ns = 0; ns < sites->getNumberOfSequences(); ns++)
  {
    auto seq = allelicAlpha->convertFromStateAlphabet(sites->sequence(ns));
    sites2.addSequence(seq->getName(), seq);
  }

  pasta.writeSequence(cerr, sites2.sequence("D"), true);

  // From Fasta
  nameSeq = "counts.fa";
  Fasta fasta;

  auto sitesf = fasta.readAlignment(nameSeq, alphaPtr);

  ProbabilisticVectorSiteContainer sitesf2(alphaPtr2);
  for (size_t ns = 0; ns < sitesf->getNumberOfSequences(); ns++)
  {
    auto seq = allelicAlpha->convertFromStateAlphabet(sitesf->sequence(ns));
    sitesf2.addSequence(seq->getName(), seq);
  }

  pasta.writeSequence(cerr, sitesf2.sequence("D"), true);


  return 0;
}
