// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/DNA.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <Bpp/Seq/SequenceTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

int main()
{
  shared_ptr<const Alphabet> alpha = AlphabetTools::DNA_ALPHABET;
  Sequence seq1("test DNA", "ATTTCG---TCGTT-AAAGCWCATGCATCGATC", alpha);
  Sequence motif1("motif", "ATTT", alpha);
  Sequence motif2("motif", "TCG", alpha);
  Sequence motif3("motif", "GATC", alpha);
  Sequence motif4("motif", "CGTC", alpha);
  Sequence motif5("motif", "CWCAT", alpha);
  Sequence motif6("motif", "CTCAT", alpha);
  Sequence motif7("motif", "AAGCA", alpha);
  size_t pos;

  cout << "--- Strict match ---" << endl;

  pos = SequenceTools::findFirstOf(seq1, motif1);
  if (pos != 0)
    return 1;
  cout << motif1.toString() << ": " << pos << endl;

  pos = SequenceTools::findFirstOf(seq1, motif2);
  if (pos != 3)
    return 1;
  cout << motif2.toString() << ": " << pos << endl;

  pos = SequenceTools::findFirstOf(seq1, motif3);
  if (pos != 29)
    return 1;
  cout << motif3.toString() << ": " << pos << endl;

  pos = SequenceTools::findFirstOf(seq1, motif4);
  if (pos != 33)
    return 1;
  cout << motif4.toString() << ": " << pos << endl;

  pos = SequenceTools::findFirstOf(seq1, motif5);
  if (pos != 19)
    return 1;
  cout << motif5.toString() << ": " << pos << endl;

  cout << "--- Degenerated match ---" << endl;

  pos = SequenceTools::findFirstOf(seq1, motif6, false);
  if (pos != 19)
    return 1;
  cout << motif6.toString() << ": " << pos << endl;

  pos = SequenceTools::findFirstOf(seq1, motif7, false);
  if (pos != 16)
    return 1;
  cout << motif7.toString() << ": " << pos << endl;

  return 0;
}
