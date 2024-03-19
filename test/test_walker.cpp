// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/App/ApplicationTools.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <Bpp/Seq/Sequence.h>
#include <Bpp/Seq/SequenceWalker.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

bool testSeq(SequenceWalker& walker, unsigned int pos, unsigned int truth)
{
  cout << 0 << endl;
  cout << walker.getSequencePosition(0) << endl;
  cout << 46 << endl;
  cout << walker.getSequencePosition(46) << endl;
  for (unsigned int i = 0; i < 1000; ++i)
  {
    ApplicationTools::displayGauge(i, 999, '=');
    size_t r = RandomTools::giveIntRandomNumberBetweenZeroAndEntry<size_t>(47);
    size_t x = walker.getSequencePosition(r);
    if (walker.getSequencePosition(pos) != truth)
    {
      cout << endl;
      cerr << r << "\t" << x << endl;
      cerr << walker.getSequencePosition(pos) << "<>" << truth << endl;
      return false;
    }
  }
  cout << endl;
  return true;
}

bool testAln(SequenceWalker& walker, unsigned int pos, unsigned int truth)
{
  cout << 0 << endl;
  cout << walker.getAlignmentPosition(0) << endl;
  cout << 26 << endl;
  cout << walker.getAlignmentPosition(26) << endl;
  for (unsigned int i = 0; i < 1000; ++i)
  {
    ApplicationTools::displayGauge(i, 999, '=');
    size_t r = RandomTools::giveIntRandomNumberBetweenZeroAndEntry<size_t>(27);
    walker.getAlignmentPosition(r);
    if (walker.getAlignmentPosition(pos) != truth)
    {
      cout << endl;
      cerr << walker.getSequencePosition(pos) << "<>" << truth << endl;
      return false;
    }
  }
  cout << endl;
  return true;
}

int main()
{
  shared_ptr<const Alphabet> alpha = AlphabetTools::RNA_ALPHABET;
  Sequence seq1("seq1", "----AUGCCG---GCGU----UUU----G--G-CCGACGUGUUUU--", alpha);
  SequenceWalker walker(seq1);

  for (size_t i = 0; i < 27; ++i)
  {
    size_t j = walker.getAlignmentPosition(i);
    cout << i << "\t" << seq1.getChar(j) << "\t" << j << endl;
  }

  cout << endl;

  if (!testAln(walker, 5, 9))
    return 1;
  if (!testAln(walker, 10, 21))
    return 1;
  if (!testAln(walker, 22, 40))
    return 1;

  cout << "_________________________________________________" << endl;

  for (size_t i = 0; i < seq1.size(); ++i)
  {
    cout << i << "\t" << seq1.getChar(i) << "\t" << walker.getSequencePosition(i) << endl;
  }


  cout << endl;

  if (!testSeq(walker, 9, 5))
    return 1;
  if (!testSeq(walker, 21, 10))
    return 1;
  if (!testSeq(walker, 40, 22))
    return 1;

  return 0;
}
