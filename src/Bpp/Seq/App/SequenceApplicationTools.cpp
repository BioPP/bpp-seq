//
// File: SequenceApplicationTools.cpp
// Authors:
//   Julien Dutheil
// Created: 2021-10-21 13:13:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#include <Bpp/App/ApplicationTools.h>
#include <Bpp/App/NumCalcApplicationTools.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Text/KeyvalTools.h>
#include <Bpp/Text/TextTools.h>
#include <algorithm>

#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/BinaryAlphabet.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../Alphabet/DefaultAlphabet.h"
#include "../Alphabet/LexicalAlphabet.h"
#include "../Container/VectorSiteContainer.h"
#include "../GeneticCode/AscidianMitochondrialGeneticCode.h"
#include "../GeneticCode/CiliateNuclearGeneticCode.h"
#include "../GeneticCode/EchinodermMitochondrialGeneticCode.h"
#include "../GeneticCode/InvertebrateMitochondrialGeneticCode.h"
#include "../GeneticCode/MoldMitochondrialGeneticCode.h"
#include "../GeneticCode/StandardGeneticCode.h"
#include "../GeneticCode/VertebrateMitochondrialGeneticCode.h"
#include "../GeneticCode/YeastMitochondrialGeneticCode.h"
#include "../Io/BppOAlignmentReaderFormat.h"
#include "../Io/BppOAlignmentWriterFormat.h"
#include "../Io/BppOAlphabetIndex1Format.h"
#include "../Io/BppOAlphabetIndex2Format.h"
#include "../Io/BppOSequenceReaderFormat.h"
#include "../Io/BppOSequenceWriterFormat.h"
#include "../Io/MaseTools.h"
#include "../SequenceTools.h"
#include "../SymbolListTools.h"
#include "SequenceApplicationTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

unique_ptr<Alphabet> SequenceApplicationTools::getAlphabet(
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  bool allowGeneric,
  int warn)
{
  unique_ptr<Alphabet> chars = nullptr;

  string alphtt = ApplicationTools::getStringParameter("alphabet", params, "DNA", suffix, suffixIsOptional, warn);

  map<string, string> args;

  string alphabet;
  KeyvalTools::parseProcedure(alphtt, alphabet, args);

  if (alphabet == "Word")
  {
    if (args.find("letter") != args.end())
    {
      args["alphabet"] = args["letter"];

      unique_ptr<const Alphabet> inAlphabet = SequenceApplicationTools::getAlphabet(args, suffix, suffixIsOptional, false, allowGeneric, warn + 1);

      if (args.find("length") == args.end())
        throw Exception("Missing length parameter for Word alphabet");

      size_t lg = TextTools::to<size_t>(args["length"]);

      chars = make_unique<WordAlphabet>(std::move(inAlphabet), lg);
    }
    else
    {
      size_t indAlph = 1;
      vector< shared_ptr<const Alphabet> > vAlph;

      while (args.find("alphabet" + TextTools::toString(indAlph)) != args.end())
      {
        map<string, string> args2;
        args2["alphabet"] = args["alphabet" + TextTools::toString(indAlph)];

        vAlph.push_back(SequenceApplicationTools::getAlphabet(args2, suffix, suffixIsOptional, verbose, allowGeneric, warn));
        indAlph++;
      }

      if (vAlph.size() == 0)
        throw Exception("SequenceApplicationTools::getAlphabet. At least one alphabet  is compulsory for Word alphabet");

      chars = make_unique<WordAlphabet>(vAlph);
    }
  }
  else if (alphabet == "RNY")
  {
    if (args.find("letter") == args.end())
      throw Exception("Missing letter alphabet for RNY alphabet");

    args["alphabet"] = args["letter"];

    shared_ptr<Alphabet> inAlphabet = SequenceApplicationTools::getAlphabet(args, suffix, suffixIsOptional, verbose, allowGeneric, warn);

    if (AlphabetTools::isNucleicAlphabet(inAlphabet.get()))
    {
      shared_ptr<Alphabet> charsTmp = std::move(chars); //unique -> shared
      chars = make_unique<RNY>(dynamic_pointer_cast<NucleicAlphabet>(charsTmp));
      alphabet = "RNY(" + alphabet + ")";
    }
    else
      throw Exception("RNY needs a Nucleic Alphabet, instead of " + args["letter"]);
  }
  else if (alphabet == "Binary")
    chars = make_unique<BinaryAlphabet>();
  else if (alphabet == "Lexicon")
  {
    vector<string> vWord = ApplicationTools::getVectorParameter<string>("words", args, ',', "()");

    if (vWord.size() == 0)
      throw Exception("SequenceApplicationTools::getAlphabet. 'words' list is missing or empty for Lexicon alphabet");

    chars = make_unique<LexicalAlphabet>(vWord);
  }
  else if (alphabet == "DNA")
  {
    bool mark = ApplicationTools::getBooleanParameter("bangAsGap", args, false, "", true, warn + 1);
    chars = make_unique<DNA>(mark);
  }
  else if (alphabet == "RNA")
  {
    bool mark = ApplicationTools::getBooleanParameter("bangAsGap", args, false, "", true, warn + 1);
    chars = make_unique<RNA>(mark);
  }
  else if (alphabet == "Protein")
    chars = make_unique<ProteicAlphabet>();
  else if (allowGeneric && alphabet == "Generic")
    chars = make_unique<DefaultAlphabet>();
  else if (alphabet == "Codon")
  {
    if (args.find("letter") == args.end())
      throw Exception("Missing 'letter' argument in Codon :" + alphabet);
    if (args.find("type") != args.end())
      throw Exception("'type' argument in Codon is deprecated and has been superseded by the 'genetic_code' option.");

    string alphnDesc = ApplicationTools::getStringParameter("letter", args, "RNA");
    string alphn;
    map<string, string> alphnArgs;
    KeyvalTools::parseProcedure(alphnDesc, alphn, alphnArgs);

    shared_ptr<const NucleicAlphabet> pnalph;
    if (alphn == "RNA")
    {
      bool mark = ApplicationTools::getBooleanParameter("bangAsGap", alphnArgs, false, "", true, warn + 1);
      pnalph = make_shared<RNA>(mark);
    }
    else if (alphn == "DNA")
    {
      bool mark = ApplicationTools::getBooleanParameter("bangAsGap", alphnArgs, false, "", true, warn + 1);
      pnalph = make_shared<DNA>(mark);
    }
    else
      throw Exception("Alphabet not known in Codon : " + alphn);

    chars = make_unique<CodonAlphabet>(pnalph);
  }
  else
    throw Exception("Alphabet not known: " + alphabet);

  if (verbose)
    ApplicationTools::displayResult("Alphabet type ", chars->getAlphabetType());
  return chars;
}

/******************************************************************************/

std::unique_ptr<GeneticCode> SequenceApplicationTools::getGeneticCode(
  std::shared_ptr<const NucleicAlphabet>& alphabet,
  const string& description)
{
  GeneticCode* geneCode;
  if (description.find("Standard") != string::npos || description.find("1") != string::npos)
    geneCode = new StandardGeneticCode(alphabet);
  else if (description.find("VertebrateMitochondrial") != string::npos || description.find("2") != string::npos)
    geneCode = new VertebrateMitochondrialGeneticCode(alphabet);
  else if (description.find("YeastMitochondrial") != string::npos || description.find("3") != string::npos)
    geneCode = new YeastMitochondrialGeneticCode(alphabet);
  else if (description.find("MoldMitochondrial") != string::npos || description.find("4") != string::npos)
    geneCode = new MoldMitochondrialGeneticCode(alphabet);
  else if (description.find("InvertebrateMitochondrial") != string::npos || description.find("5") != string::npos)
    geneCode = new InvertebrateMitochondrialGeneticCode(alphabet);
  else if (description.find("CiliateNuclear") != string::npos || description.find("6") != string::npos)
    geneCode = new CiliateNuclearGeneticCode(alphabet);
  else if (description.find("EchinodermMitochondrial") != string::npos || description.find("9") != string::npos)
    geneCode = new EchinodermMitochondrialGeneticCode(alphabet);
  else if (description.find("AscidianMitochondrial") != string::npos || description.find("13") != string::npos)
    geneCode = new AscidianMitochondrialGeneticCode(alphabet);
  else
    throw Exception("Unknown GeneticCode: " + description);
  return unique_ptr<GeneticCode>(geneCode);
}

/******************************************************************************/

unique_ptr<AlphabetIndex1> SequenceApplicationTools::getAlphabetIndex1(shared_ptr<const Alphabet>& alphabet, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex1Format reader(alphabet, message, verbose);
  return reader.read(description);
}

unique_ptr<AlphabetIndex2> SequenceApplicationTools::getAlphabetIndex2(shared_ptr<const Alphabet>& alphabet, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex2Format reader(alphabet, message, verbose);
  return reader.read(description);
}

unique_ptr<AlphabetIndex1> SequenceApplicationTools::getAlphabetIndex1(shared_ptr<const CodonAlphabet>& alphabet, shared_ptr<const GeneticCode>& gencode, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex1Format reader(alphabet, gencode, message, verbose);
  return reader.read(description);
}

unique_ptr<AlphabetIndex2> SequenceApplicationTools::getAlphabetIndex2(shared_ptr<const CodonAlphabet>& alphabet, shared_ptr<const GeneticCode>& gencode, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex2Format reader(alphabet, gencode, message, verbose);
  return reader.read(description);
}

/******************************************************************************/

std::unique_ptr<BasicSequenceContainer> SequenceApplicationTools::getSequenceContainer(
  std::shared_ptr<const Alphabet>& alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOSequenceReaderFormat bppoReader(warn);
  unique_ptr<IBasicSequence> iSeq(bppoReader.read(sequenceFormat));
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iSeq->getFormatName());
  }
  std::unique_ptr<BasicSequenceContainer> sequences = iSeq->readSequences(sequenceFilePath, alpha);

  // Apply sequence selection:
  restrictSelectedSequencesByName(*sequences, params, suffix, suffixIsOptional, verbose, warn);

  return sequences;
}


/******************************************************************************/

map<size_t, unique_ptr<BasicVectorSiteContainer> >
SequenceApplicationTools::getBasicSiteContainers(
  std::shared_ptr<const Alphabet>& alpha,
  const map<string, string>& params,
  const string& prefix,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  vector<string> vContName = ApplicationTools::matchingParameters(prefix + "data*", params);

  map<size_t, unique_ptr<BasicVectorSiteContainer> > mCont;

  for (size_t nT = 0; nT < vContName.size(); nT++)
  {
    size_t poseq = vContName[nT].find("=");
    size_t num = 0;
    size_t len = (prefix + "data").size();

    string suff = vContName[nT].substr(len, poseq - len);

    if (TextTools::isDecimalInteger(suff, '$'))
      num = TextTools::to<size_t>(suff);
    else
      num = 1;

    string contDesc = ApplicationTools::getStringParameter(vContName[nT], params, "", suffix, suffixIsOptional);

    string contName;

    map<string, string> args;

    KeyvalTools::parseProcedure(contDesc, contName, args);

    map<string, string> args2;

    if (contName == "alignment")
    {
      string format;

      if (args.find("file") != args.end())
        args2["input.sequence.file"] = args["file"];
      else
        args2["input.sequence.file"] = "";

      if (args.find("format") != args.end())
        args2["input.sequence.format"] = args["format"];

      if (args.find("selection") != args.end())
        args2["input.site.selection"] = args["selection"];

      if (args.find("sites_to_use") != args.end())
        args2["input.sequence.sites_to_use"] = args["sites_to_use"];

      if (args.find("max_gap_allowed") != args.end())
        args2["input.sequence.max_gap_allowed"] = args["max_gap_allowed"];

      if (args.find("max_unresolved_allowed") != args.end())
        args2["input.sequence.max_unresolved_allowed"] = args["max_unresolved_allowed"];

      if (args.find("remove_stop_codons") != args.end())
        args2["input.sequence.remove_stop_codons"] = args["remove_stop_codons"];

      args2["genetic_code"] = ApplicationTools::getStringParameter("genetic_code", params, "", "", true, (AlphabetTools::isCodonAlphabet(alpha.get()) ? 0 : 1));

      ApplicationTools::displayMessage("");
      ApplicationTools::displayMessage("Data " + TextTools::toString(num));

      auto vsC = getBasicSiteContainer(alpha, args2, "", true, verbose, warn);
      vsC = getSitesToAnalyse(*vsC, args2, "", true, false);

      if (mCont.find(num) != mCont.end())
      {
        ApplicationTools::displayWarning("Alignment " + TextTools::toString(num) + " already assigned, replaced by new one.");
      }
      mCont.emplace(num, std::move(vsC));
    }
  }

  return mCont;
}

/******************************************************************************/

map<size_t, unique_ptr<ProbabilisticVectorSiteContainer> >
SequenceApplicationTools::getProbabilisticSiteContainers(
  std::shared_ptr<const Alphabet>& alpha,
  const map<string, string>& params,
  const string& prefix,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  vector<string> vContName = ApplicationTools::matchingParameters(prefix + "data*", params);

  map<size_t, unique_ptr<ProbabilisticVectorSiteContainer> > mCont;

  for (size_t nT = 0; nT < vContName.size(); nT++)
  {
    size_t poseq = vContName[nT].find("=");
    size_t num = 0;
    size_t len = (prefix + "data").size();

    string suff = vContName[nT].substr(len, poseq - len);

    if (TextTools::isDecimalInteger(suff, '$'))
      num = TextTools::to<size_t>(suff);
    else
      num = 1;

    string contDesc = ApplicationTools::getStringParameter(vContName[nT], params, "", suffix, suffixIsOptional);

    string contName;

    map<string, string> args;

    KeyvalTools::parseProcedure(contDesc, contName, args);

    map<string, string> args2;

    if (contName == "alignment")
    {
      string format;

      if (args.find("file") != args.end())
        args2["input.sequence.file"] = args["file"];
      else
        args2["input.sequence.file"] = "";

      if (args.find("format") != args.end())
        args2["input.sequence.format"] = args["format"];

      if (args.find("selection") != args.end())
        args2["input.site.selection"] = args["selection"];

      if (args.find("sites_to_use") != args.end())
        args2["input.sequence.sites_to_use"] = args["sites_to_use"];

      if (args.find("max_gap_allowed") != args.end())
        args2["input.sequence.max_gap_allowed"] = args["max_gap_allowed"];

      if (args.find("max_unresolved_allowed") != args.end())
        args2["input.sequence.max_unresolved_allowed"] = args["max_unresolved_allowed"];

      if (args.find("remove_stop_codons") != args.end())
        args2["input.sequence.remove_stop_codons"] = args["remove_stop_codons"];

      args2["genetic_code"] = ApplicationTools::getStringParameter("genetic_code", params, "", "", true, (AlphabetTools::isCodonAlphabet(alpha.get()) ? 0 : 1));

      ApplicationTools::displayMessage("");
      ApplicationTools::displayMessage("Data " + TextTools::toString(num));

      auto vsC = getProbabilisticSiteContainer(alpha, args2, "", true, verbose, warn);
      vsC = getSitesToAnalyse(*vsC, args2, "", true, false);

      if (mCont.find(num) != mCont.end())
      {
        ApplicationTools::displayWarning("Alignment " + TextTools::toString(num) + " already assigned, replaced by new one.");
      }
      mCont.emplace(num, move(vsC));
    }
  }
  return mCont;
}

/******************************************************************************/

std::unique_ptr<BasicVectorSiteContainer> SequenceApplicationTools::getBasicSiteContainer(
  std::shared_ptr<const Alphabet>& alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOAlignmentReaderFormat bppoReader(warn);
  unique_ptr<IBasicAlignment> iAln(bppoReader.read(sequenceFormat));
  map<string, string> args(bppoReader.getUnparsedArguments());
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iAln->getFormatName());
  }

  shared_ptr<const Alphabet> alpha2;
  if (AlphabetTools::isRNYAlphabet(alpha.get()))
    alpha2 = dynamic_pointer_cast<const RNY>(alpha)->getLetterAlphabet();
  else
    alpha2 = alpha;

  auto sites2 = make_unique<BasicVectorSiteContainer>(*(iAln->readAlignment(sequenceFilePath, alpha2))); //We copy into a VectorSiteContainer, as most readers will generate an AlignedSequenceContainer)

  auto sites = make_unique<BasicVectorSiteContainer>();

  if (AlphabetTools::isRNYAlphabet(alpha.get()))
  {
    sites = make_unique<BasicVectorSiteContainer>(alpha);
    for (size_t i = 0; i < sites2->getNumberOfSequences(); ++i)
    {
      auto seqP = SequenceTools::RNYslice(sites2->getSequence(i));
      sites->addSequence(sites2->getSequenceKey(i), seqP);
    }
  }
  else
    sites = move(sites2);

  // Look for site selection:
  if (iAln->getFormatName() == "MASE file")
  {
    // getting site set:
    string siteSet = ApplicationTools::getStringParameter("siteSelection", args, "none", suffix, suffixIsOptional, warn + 1);
    if (siteSet != "none")
    {
      unique_ptr<BasicVectorSiteContainer> selectedSites;;
      try
      {
        auto sel = MaseTools::getSelectedSites(*sites, siteSet);
        selectedSites = move(sel);
        if (verbose)
          ApplicationTools::displayResult("Set found", TextTools::toString(siteSet) + " sites.");
      }
      catch (IOException& ioe)
      {
        throw ioe;
      }
      if (selectedSites->getNumberOfSites() == 0)
      {
        throw Exception("Site set '" + siteSet + "' is empty.");
      }
      sites = move(selectedSites);
    }
  }
  else
  {
    // getting site set:
    size_t nbSites = sites->getNumberOfSites();

    string siteSet = ApplicationTools::getStringParameter("input.site.selection", params, "none", suffix, suffixIsOptional, warn + 1);

    unique_ptr<BasicVectorSiteContainer> selectedSites;;
    if (siteSet != "none")
    {
      if (siteSet[0] == '(')
        siteSet = siteSet.substr(1, siteSet.size() - 2);

      vector<size_t> vSite;
      try
      {
        vector<int> vSite1 = NumCalcApplicationTools::seqFromString(siteSet, ",", ":");
        for (size_t i = 0; i < vSite1.size(); ++i)
        {
          int x = (vSite1[i] >= 0 ? vSite1[i] : static_cast<int>(nbSites) + vSite1[i] + 1);
          if (x <= (int)nbSites)
          {
            if (x > 0)
              vSite.push_back(static_cast<size_t>(x - 1));
            else
              throw Exception("SequenceApplicationTools::getSiteContainer(). Incorrect null index: " + TextTools::toString(x));
          }
          else
            throw Exception("SequenceApplicationTools::getSiteContainer(). Too large index: " + TextTools::toString(x));
        }
        auto sel = SiteContainerTools::getSelectedSites(*sites, vSite);
        selectedSites = move(sel);
        selectedSites->reindexSites();
      }
      catch (Exception& e)
      {
        string seln;
        map<string, string> selArgs;
        KeyvalTools::parseProcedure(siteSet, seln, selArgs);
        if (seln == "Sample")
        {
          size_t n = ApplicationTools::getParameter<size_t>("n", selArgs, nbSites, "", true, warn + 1);
          bool replace = ApplicationTools::getBooleanParameter("replace", selArgs, false, "", true, warn + 1);

          vSite.resize(n);
          vector<size_t> vPos;
          for (size_t p = 0; p < nbSites; ++p)
          {
            vPos.push_back(p);
          }

          RandomTools::getSample(vPos, vSite, replace);

          auto sel = SiteContainerTools::getSelectedSites(*sites, vSite);
          selectedSites = move(sel);
          if (replace)
            selectedSites->reindexSites();
        }
        else
          throw Exception("Unknown site selection description: " + siteSet);
      }

      if (verbose)
        ApplicationTools::displayResult("Selected sites", TextTools::toString(siteSet));

      if (selectedSites && (selectedSites->getNumberOfSites() == 0))
      {
        throw Exception("Site set '" + siteSet + "' is empty.");
      }
      sites = move(selectedSites);
    }
  }
  // Apply sequence selection:
  restrictSelectedSequencesByName(*sites, params, suffix, suffixIsOptional, verbose, warn);

  return sites;
}

/******************************************************************************/

unique_ptr<ProbabilisticVectorSiteContainer> SequenceApplicationTools::getProbabilisticSiteContainer(
  shared_ptr<const Alphabet>& alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOAlignmentReaderFormat bppoReader(warn);

  unique_ptr<IProbabilisticAlignment> iProbAln(bppoReader.readProbabilistic(sequenceFormat));

  map<string, string> args(bppoReader.getUnparsedArguments());
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iProbAln->getFormatName());
  }

  shared_ptr<const Alphabet> alpha2;
  if (AlphabetTools::isRNYAlphabet(alpha.get()))
    alpha2 = dynamic_pointer_cast<const RNY>(alpha)->getLetterAlphabet();
  else
    alpha2 = alpha;

  auto sites = make_unique<ProbabilisticVectorSiteContainer>(*(iProbAln->readAlignment(sequenceFilePath, alpha2))); //We copy into a VectorSiteContainer

  // getting site set:
  size_t nbSites = sites->getNumberOfSites();

  string siteSet = ApplicationTools::getStringParameter("input.site.selection", params, "none", suffix, suffixIsOptional, warn + 1);

  unique_ptr<ProbabilisticVectorSiteContainer> selectedSites;;
  if (siteSet != "none")
  {
    if (siteSet[0] == '(')
      siteSet = siteSet.substr(1, siteSet.size() - 2);

    vector<size_t> vSite;
    try
    {
      vector<int> vSite1 = NumCalcApplicationTools::seqFromString(siteSet, ",", ":");

      for (auto ps : vSite1)
      {
        int x = (ps >= 0 ? ps : static_cast<int>(nbSites) + ps + 1);
        if (x <= (int)nbSites)
        {
          if (x > 0)
            vSite.push_back(static_cast<size_t>(x - 1));
          else
            throw Exception("SequenceApplicationTools::getSiteContainer(). Incorrect null index: " + TextTools::toString(x));
        }
        else
          throw Exception("SequenceApplicationTools::getSiteContainer(). Too large index: " + TextTools::toString(x));
      }
      selectedSites = SiteContainerTools::getSelectedSites(*sites, vSite);
      selectedSites->reindexSites();
    }
    catch (Exception& e)
    {
      string seln;
      map<string, string> selArgs;
      KeyvalTools::parseProcedure(siteSet, seln, selArgs);
      if (seln == "Sample")
      {
        size_t n = ApplicationTools::getParameter<size_t>("n", selArgs, nbSites, "", true, warn + 1);
        bool replace = ApplicationTools::getBooleanParameter("replace", selArgs, false, "", true, warn + 1);

        vSite.resize(n);
        vector<size_t> vPos;
        for (size_t p = 0; p < nbSites; ++p)
        {
          vPos.push_back(p);
        }

        RandomTools::getSample(vPos, vSite, replace);

        selectedSites = SiteContainerTools::getSelectedSites(*sites, vSite);
        if (replace)
          selectedSites->reindexSites();
      }
      else
        throw Exception("Unknown site selection description: " + siteSet);
    }

    if (verbose)
      ApplicationTools::displayResult("Selected sites", TextTools::toString(siteSet));

    if (selectedSites && (selectedSites->getNumberOfSites() == 0))
    {
      throw Exception("Site set '" + siteSet + "' is empty.");
    }
    sites = move(selectedSites);
  }

  return sites;
}

/******************************************************************************/

void SequenceApplicationTools::restrictSelectedSequencesByName(
  BasicSequenceContainer& allSequences,
  const map<std::string, std::string>& params,
  string suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string optionKeep = ApplicationTools::getStringParameter("input.sequence.keep_names", params, "all", suffix, suffixIsOptional, warn);
  if (optionKeep != "all") {
    vector<string> selection = ApplicationTools::getVectorParameter<string>("input.sequence.keep_names", params, ',', optionKeep, suffix, suffixIsOptional, warn);
    sort(selection.begin(), selection.end());
    for (const auto& name: allSequences.getSequenceNames()) {
      if (! binary_search(selection.begin(), selection.end(), name)) {
        allSequences.removeSequence(name);
        if (verbose) {
          ApplicationTools::displayResult("Discard sequence", name);
        }
      }
    }
  }
  string optionRemove = ApplicationTools::getStringParameter("input.sequence.remove_names", params, "none", suffix, suffixIsOptional, warn);
  if (optionRemove != "none") {
    vector<string> selection = ApplicationTools::getVectorParameter<string>("input.sequence.remove_names", params, ',', optionRemove, suffix, suffixIsOptional, warn);
    vector<string> seqNames = allSequences.getSequenceNames();
    sort(seqNames.begin(), seqNames.end());
    for (const auto& name: selection) {
      if (binary_search(seqNames.begin(), seqNames.end(), name)) {
        allSequences.removeSequence(name);
        if (verbose) {
          ApplicationTools::displayResult("Discard sequence", name);
        }
      } 
      else {
        throw SequenceNotFoundException("No sequence with the specified name was found.", name);
      }
    }
  }
}

/******************************************************************************/

void SequenceApplicationTools::writeSequenceFile(
  const BasicSequenceContainer& sequences,
  const map<string, string>& params,
  const string& suffix,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("output.sequence.file", params, true, false, suffix, false, "none", warn);
  string sequenceFormat   = ApplicationTools::getStringParameter("output.sequence.format", params, "Fasta", suffix, false, warn);
  BppOSequenceWriterFormat bppoWriter(warn);
  unique_ptr<OBasicSequence> oSeq(bppoWriter.read(sequenceFormat));
  if (verbose)
  {
    ApplicationTools::displayResult("Output sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Output sequence format " + suffix, oSeq->getFormatName());
  }

  // Write sequences:
  oSeq->writeSequences(sequenceFilePath, sequences, true);
}

/******************************************************************************/

void SequenceApplicationTools::writeAlignmentFile(
  const BasicSiteContainer& sites,
  const map<string, string>& params,
  const string& suffix,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("output.sequence.file", params, true, false, suffix, false, "none", warn);
  string sequenceFormat   = ApplicationTools::getStringParameter("output.sequence.format", params, "Fasta", suffix, false, warn);
  BppOAlignmentWriterFormat bppoWriter(warn);
  unique_ptr<OBasicAlignment> oAln(bppoWriter.read(sequenceFormat));
  if (verbose)
  {
    ApplicationTools::displayResult("Output alignment file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Output alignment format " + suffix, oAln->getFormatName());
  }

  // Write sequences:
  oAln->writeAlignment(sequenceFilePath, sites, true);
}

/******************************************************************************/
