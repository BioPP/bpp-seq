// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Numeric/DataTable.h>
#include <fstream>

#include "../StringSequenceTools.h"
#include "Csv.h"


using namespace bpp;
using namespace std;


void Csv::appendSequencesFromStream(istream& input, SequenceContainerInterface& vsc) const
{
  if (!input)
    throw IOException("Csv::appendFromStream: can't read from istream input");

  auto datatable = DataTable::read(input, sep_, header_, rowNames_);

  auto seqnames = datatable->getColumnNames();

  auto alphaPtr = vsc.getAlphabet();

  for (const auto& seqname : seqnames)
  {
    const auto& content = datatable->getColumn(seqname);
    auto tmpseq = make_unique<Sequence>("", "", alphaPtr);
    tmpseq->setName(seqname);
    tmpseq->setContent(content);
    vsc.addSequence(seqname, tmpseq);
  }
}

/******************************************************************************/

void Csv::writeSequences(ostream& output, const SequenceContainerInterface& sc) const
{
  if (!output)
    throw IOException("Csv::write: can't write to ostream output");

  auto seqnames = sc.getSequenceNames();
  DataTable datatable(seqnames);

  auto alphaPtr = sc.getAlphabet();

  auto len = sc.sequence(seqnames[0]).size();
  
  // Main loop : for all sequences in vector container
  for (size_t i = 0; i < len; ++i)
  {
    vector<string> v;
    for (const auto& name : seqnames)
      v.push_back(sc.sequence(name).getChar(i));

    datatable.addRow(v);
  }

  DataTable::write(datatable, output, sep_);
}

