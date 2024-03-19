// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Seq/Alphabet/RNA.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>
#include <Bpp/Seq/SequenceTools.h>
#include <iostream>

using namespace bpp;
using namespace std;

unique_ptr<Sequence> getRandomSequence(shared_ptr<const Alphabet> alphabet, size_t size)
{
  string seq = "";
  for (size_t i = 0; i < size; ++i)
  {
    seq += alphabet->intToChar(RandomTools::giveIntRandomNumberBetweenZeroAndEntry(static_cast<int>(alphabet->getSize())));
  }
  return make_unique<Sequence>("random seq", seq, alphabet);
}

int test(shared_ptr<const Alphabet> alphabet, size_t size, unsigned int rep)
{
  double n01 = 0;
  double n05 = 0;
  double n10 = 0;
  double n50 = 0;

  // ofstream out("pvalues.txt", ios::out);
  for (unsigned int i = 0; i < rep; ++i)
  {
    ApplicationTools::displayGauge(i, rep - 1);
    auto seq1 = getRandomSequence(alphabet, size);
    auto seq2 = getRandomSequence(alphabet, size);
    auto test = SequenceTools::bowkerTest(*seq1, *seq2);
    double p = test->getPValue();
    if (p <= 0.01)
      n01++;
    if (p <= 0.05)
      n05++;
    if (p <= 0.10)
      n10++;
    if (p <= 0.50)
      n50++;
    // out << p << endl;
  }
  // out.close();

  cout << n01 / rep <<
    "\t" << n05 / rep <<
    "\t" << n10 / rep <<
    "\t" << n50 / rep << endl;

  if (abs(n01 * 100 / rep - 1 ) > 1)
    return 1;
  if (abs(n05 * 100 / rep - 5 ) > 1)
    return 1;
  if (abs(n10 * 100 / rep - 10) > 1)
    return 1;
  if (abs(n50 * 100 / rep - 50) > 1)
    return 1;
  return 0;
}

int main()
{
  shared_ptr<const Alphabet> alpha = AlphabetTools::RNA_ALPHABET;
  auto seq1 = make_unique<Sequence>("seq1", "----AUGCCG---GCGU----UUU----G--G-CCGACGUGUUUU--", alpha);
  auto seq2 = make_unique<Sequence>("seq2", "---GAAGGCG---G-GU----UUU----GC-GACCGACG--UUUU--", alpha);
  auto btest = SequenceTools::bowkerTest(*seq1, *seq2);
  cout << btest->getStatistic() << "\t" << btest->getPValue() << endl;

  test(AlphabetTools::DNA_ALPHABET, 1000, 5000);
  test(AlphabetTools::PROTEIN_ALPHABET, 20000, 5000);

  return 0;
}
