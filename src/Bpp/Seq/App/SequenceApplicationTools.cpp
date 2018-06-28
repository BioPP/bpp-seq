//
// File: SequenceApplicationTools.cpp
// Created by: Julien Dutheil
// Created on: Fri Oct 21 13:13
// from file old ApplicationTools.h created on Sun Dec 14 09:36:26 2003
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for sequences analysis.

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

#include "SequenceApplicationTools.h"
#include "../Alphabet/BinaryAlphabet.h"
#include "../Alphabet/LexicalAlphabet.h"
#include "../Alphabet/DefaultAlphabet.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../Alphabet/AlphabetTools.h"
#include "../GeneticCode/EchinodermMitochondrialGeneticCode.h"
#include "../GeneticCode/InvertebrateMitochondrialGeneticCode.h"
#include "../GeneticCode/StandardGeneticCode.h"
#include "../GeneticCode/VertebrateMitochondrialGeneticCode.h"
#include "../GeneticCode/YeastMitochondrialGeneticCode.h"
#include "../GeneticCode/AscidianMitochondrialGeneticCode.h"
#include "../GeneticCode/MoldMitochondrialGeneticCode.h"
#include "../Io/BppOSequenceReaderFormat.h"
#include "../Io/BppOAlignmentReaderFormat.h"
#include "../Io/BppOSequenceWriterFormat.h"
#include "../Io/BppOAlignmentWriterFormat.h"
#include "../Io/BppOAlphabetIndex1Format.h"
#include "../Io/BppOAlphabetIndex2Format.h"
#include "../Io/MaseTools.h"
#include "../SymbolListTools.h"
#include "../SequenceTools.h"
#include "../Container/VectorProbabilisticSiteContainer.h"
#include "../Container/OrderedSequenceContainer.h"
#include <Bpp/App/ApplicationTools.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/KeyvalTools.h>
#include <Bpp/App/NumCalcApplicationTools.h>
#include <Bpp/Numeric/Random/RandomTools.h>

using namespace bpp;
using namespace std;

/******************************************************************************/

Alphabet* SequenceApplicationTools::getAlphabet(
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  bool allowGeneric,
  int warn)
{
  Alphabet* chars = 0;
  
  string alphtt = ApplicationTools::getStringParameter("alphabet", params, "DNA", suffix, suffixIsOptional, warn);

  map<string, string> args;

  string alphabet;
  KeyvalTools::parseProcedure(alphtt, alphabet, args);

  if (alphabet == "Word")
  {
    if (args.find("letter") != args.end())
    {
      args["alphabet"]=args["letter"];
      
      Alphabet* inAlphabet = SequenceApplicationTools::getAlphabet(args, suffix, suffixIsOptional, false, allowGeneric, warn+1);
      
      if (args.find("length") == args.end())
        throw Exception("Missing length parameter for Word alphabet");

      size_t lg = TextTools::to<unsigned int>(args["length"]);

      chars = new WordAlphabet(inAlphabet, lg);
    }
    else
    {
      size_t indAlph = 1;
      vector<const Alphabet*> vAlph;

      while (args.find("alphabet" + TextTools::toString(indAlph)) != args.end())
      {
        map<string,string> args2;
        args2["alphabet"]=args["alphabet" + TextTools::toString(indAlph)];

        vAlph.push_back(SequenceApplicationTools::getAlphabet(args2, suffix, suffixIsOptional, verbose, allowGeneric, warn));
        indAlph++;
      }

      if (vAlph.size() == 0)
        throw Exception("SequenceApplicationTools::getAlphabet. At least one alphabet  is compulsory for Word alphabet");

      chars = new WordAlphabet(vAlph);
    }
  }
  else if (alphabet == "RNY")
  {
    if (args.find("letter") == args.end())
      throw Exception("Missing letter alphabet for RNY alphabet");

    args["alphabet"]=args["letter"];
      
    Alphabet* inAlphabet = SequenceApplicationTools::getAlphabet(args, suffix, suffixIsOptional, verbose, allowGeneric, warn);

    if (AlphabetTools::isNucleicAlphabet(inAlphabet))
    {
      chars = new RNY(*(dynamic_cast<NucleicAlphabet*>(chars)));
      alphabet = "RNY(" + alphabet + ")";
    }
    else
      throw Exception("RNY needs a Nucleic Alphabet, instead of " + args["letter"]);
  }
  else if (alphabet == "Binary")
    chars = new BinaryAlphabet();
  else if (alphabet == "Lexicon")
  {
    vector<string> vWord = ApplicationTools::getVectorParameter<string>("words", args, ',', "()");

    if (vWord.size()==0)
      throw Exception("SequenceApplicationTools::getAlphabet. 'words' list is missing or empty for Lexicon alphabet");
    
    chars = new LexicalAlphabet(vWord);
  }
  else if (alphabet == "DNA")
  {
    bool mark = ApplicationTools::getBooleanParameter("bangAsGap", args, false, "", true, warn + 1);
    chars = new DNA(mark);
  }
  else if (alphabet == "RNA")
  {
    bool mark = ApplicationTools::getBooleanParameter("bangAsGap", args, false, "", true, warn + 1);
    chars = new RNA(mark);
  }
  else if (alphabet == "Protein")
    chars = new ProteicAlphabet();
  else if (allowGeneric && alphabet == "Generic")
    chars = new DefaultAlphabet();
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

    NucleicAlphabet* pnalph;
    if (alphn == "RNA")
    {
      bool mark = ApplicationTools::getBooleanParameter("bangAsGap", alphnArgs, false, "", true, warn + 1);
      pnalph = new RNA(mark);
    }
    else if (alphn == "DNA")
    {
      bool mark = ApplicationTools::getBooleanParameter("bangAsGap", alphnArgs, false, "", true, warn + 1);
      pnalph = new DNA(mark);
    }
    else
      throw Exception("Alphabet not known in Codon : " + alphn);

    chars = new CodonAlphabet(pnalph);
  }
  else
    throw Exception("Alphabet not known: " + alphabet);

  if (verbose)
    ApplicationTools::displayResult("Alphabet type ", chars->getAlphabetType());
  return chars;
}

/******************************************************************************/

GeneticCode* SequenceApplicationTools::getGeneticCode(
  const NucleicAlphabet* alphabet,
  const string& description)
{
  GeneticCode* geneCode;
  if (description.find("EchinodermMitochondrial") != string::npos || description.find("9") != string::npos)
    geneCode = new EchinodermMitochondrialGeneticCode(alphabet);
  else if (description.find("InvertebrateMitochondrial") != string::npos || description.find("5") != string::npos)
    geneCode = new InvertebrateMitochondrialGeneticCode(alphabet);
  else if (description.find("Standard") != string::npos || description.find("1") != string::npos)
    geneCode = new StandardGeneticCode(alphabet);
  else if (description.find("VertebrateMitochondrial") != string::npos || description.find("2") != string::npos)
    geneCode = new VertebrateMitochondrialGeneticCode(alphabet);
  else if (description.find("YeastMitochondrial") != string::npos || description.find("3") != string::npos)
    geneCode = new YeastMitochondrialGeneticCode(alphabet);
  else if (description.find("AscidianMitochondrial") != string::npos || description.find("13") != string::npos)
    geneCode = new AscidianMitochondrialGeneticCode(alphabet);
  else if (description.find("MoldMitochondrial") != string::npos || description.find("4") != string::npos)
    geneCode = new MoldMitochondrialGeneticCode(alphabet);
  else
    throw Exception("Unknown GeneticCode: " + description);
  return geneCode;
}

/******************************************************************************/

AlphabetIndex1* SequenceApplicationTools::getAlphabetIndex1(const Alphabet* alphabet, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex1Format reader(alphabet, message, verbose);
  return reader.read(description);
}

AlphabetIndex2* SequenceApplicationTools::getAlphabetIndex2(const Alphabet* alphabet, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex2Format reader(alphabet, message, verbose);
  return reader.read(description);
}

AlphabetIndex1* SequenceApplicationTools::getAlphabetIndex1(const CodonAlphabet* alphabet, const GeneticCode* gencode, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex1Format reader(alphabet, gencode, message, verbose);
  return reader.read(description);
}

AlphabetIndex2* SequenceApplicationTools::getAlphabetIndex2(const CodonAlphabet* alphabet, const GeneticCode* gencode, const string& description, const string& message, bool verbose)
{
  BppOAlphabetIndex2Format reader(alphabet, gencode, message, verbose);
  return reader.read(description);
}

/******************************************************************************/
SequenceContainer* SequenceApplicationTools::getSequenceContainer(
  const Alphabet* alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOSequenceReaderFormat bppoReader(warn);
  unique_ptr<ISequence> iSeq(bppoReader.read(sequenceFormat));
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iSeq->getFormatName());
  }
  SequenceContainer* sequences = iSeq->readSequences(sequenceFilePath, alpha);

  return sequences;
}


/******************************************************************************/

map<size_t, SiteContainer*> SequenceApplicationTools::getSiteContainers(
  const Alphabet* alpha,
  const map<string, string>& params,
  const string& prefix,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  vector<string> vContName = ApplicationTools::matchingParameters(prefix + "data*", params);

  map<size_t, SiteContainer*> mCont;

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

      args2["genetic_code"] = ApplicationTools::getStringParameter("genetic_code", params, "", "", true, (dynamic_cast<const CodonAlphabet*>(alpha)?0:1));

      ApplicationTools::displayMessage("");
      ApplicationTools::displayMessage("Data " + TextTools::toString(num));

      VectorSiteContainer* vsC = dynamic_cast<VectorSiteContainer*>(getSiteContainer(alpha, args2, "", true, verbose, warn));

      VectorSiteContainer* vsC2 = dynamic_cast<VectorSiteContainer*>(getSitesToAnalyse(*vsC, args2, "", true, false));

      delete vsC;

      if (mCont.find(num) != mCont.end())
      {
        ApplicationTools::displayWarning("Alignment " + TextTools::toString(num) + " already assigned, replaced by new one.");
        delete mCont[num];
      }
      mCont[num] = vsC2;
    }
  }

  return mCont;
}

map<size_t, AlignedValuesContainer*> SequenceApplicationTools::getAlignedContainers(
  const Alphabet* alpha,
  const map<string, string>& params,
  const string& prefix,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  vector<string> vContName = ApplicationTools::matchingParameters(prefix + "data*", params);

  map<size_t, AlignedValuesContainer*> mCont;

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

      args2["genetic_code"] = ApplicationTools::getStringParameter("genetic_code", params, "", "", true, (dynamic_cast<const CodonAlphabet*>(alpha)?0:1));

      ApplicationTools::displayMessage("");
      ApplicationTools::displayMessage("Data " + TextTools::toString(num));

      AlignedValuesContainer* vsC = getAlignedContainer(alpha, args2, "", true, verbose, warn);

      AlignedValuesContainer* vsC2 = getSitesToAnalyse(*vsC, args2, "", true, false);
      delete vsC;

      if (mCont.find(num) != mCont.end())
      {
        ApplicationTools::displayWarning("Alignment " + TextTools::toString(num) + " already assigned, replaced by new one.");
        delete mCont[num];
      }
      mCont[num] = vsC2;
   }
    
  }
  return mCont;
}

VectorSiteContainer* SequenceApplicationTools::getSiteContainer(
  const Alphabet* alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOAlignmentReaderFormat bppoReader(warn);
  unique_ptr<IAlignment> iAln(bppoReader.read(sequenceFormat));
  map<string, string> args(bppoReader.getUnparsedArguments());
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iAln->getFormatName());
  }

  const Alphabet* alpha2;
  if (AlphabetTools::isRNYAlphabet(alpha))
    alpha2 = &dynamic_cast<const RNY*>(alpha)->getLetterAlphabet();
  else
    alpha2 = alpha;

  const SiteContainer* seqCont = iAln->readAlignment(sequenceFilePath, alpha2);

  VectorSiteContainer* sites2 = new VectorSiteContainer(*seqCont);
  
  delete seqCont;
  
  VectorSiteContainer* sites;
  
  if (AlphabetTools::isRNYAlphabet(alpha))
  {
    const SequenceTools ST;
    sites = new VectorSiteContainer(alpha);
    for (unsigned int i = 0; i < sites2->getNumberOfSequences(); i++)
    {
      sites->addSequence(*(ST.RNYslice(sites2->getSequence(i))));
     }
    delete sites2;
  }
  else
    sites = sites2;
  
  
  // Look for site selection:
  if (iAln->getFormatName() == "MASE file")
  {
    // getting site set:
    string siteSet = ApplicationTools::getStringParameter("siteSelection", args, "none", suffix, suffixIsOptional, warn + 1);
    if (siteSet != "none")
    {
      VectorSiteContainer* selectedSites;
      try
      {
        selectedSites = dynamic_cast<VectorSiteContainer*>(MaseTools::getSelectedSites(*sites, siteSet));
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
      delete sites;
      sites = selectedSites;
    }
  }
  else
  {
    // getting site set:
    size_t nbSites = sites->getNumberOfSites();

    string siteSet = ApplicationTools::getStringParameter("input.site.selection", params, "none", suffix, suffixIsOptional, warn + 1);

    VectorSiteContainer* selectedSites = 0;
    if (siteSet != "none")
    {
      if(siteSet[0]=='(')
        siteSet=siteSet.substr(1,siteSet.size()-2);

      vector<size_t> vSite;
      try
      {
        vector<int> vSite1 = NumCalcApplicationTools::seqFromString(siteSet,",",":");
        for (size_t i = 0; i < vSite1.size(); ++i)
        {
          int x = (vSite1[i] >= 0 ? vSite1[i] : static_cast<int>(nbSites) + vSite1[i]+ 1);
          if (x<=(int)nbSites)
          {
            if (x > 0)
              vSite.push_back(static_cast<size_t>(x - 1));
            else
              throw Exception("SequenceApplicationTools::getSiteContainer(). Incorrect null index: " + TextTools::toString(x));
          }
          else
            throw Exception("SequenceApplicationTools::getSiteContainer(). Too large index: " + TextTools::toString(x));
        }
        selectedSites = dynamic_cast<VectorSiteContainer*>(SiteContainerTools::getSelectedSites(*sites, vSite));
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

          selectedSites = dynamic_cast<VectorSiteContainer*>(SiteContainerTools::getSelectedSites(*sites, vSite));
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
      delete sites;
      sites = selectedSites;
    }
  }
  return sites;
}

AlignedValuesContainer* SequenceApplicationTools::getAlignedContainer(
  const Alphabet* alpha,
  const map<string, string>& params,
  const string& suffix,
  bool suffixIsOptional,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("input.sequence.file", params, true, true, suffix, suffixIsOptional, "none", warn);
  string sequenceFormat = ApplicationTools::getStringParameter("input.sequence.format", params, "Fasta()", suffix, suffixIsOptional, warn);
  BppOAlignmentReaderFormat bppoReader(warn);

  unique_ptr<IAlignment> iAln;
  unique_ptr<IProbabilisticAlignment> iProbAln;

  try{
    iAln.reset(bppoReader.read(sequenceFormat));
  }
  catch (Exception& e)
  {
    iProbAln.reset(bppoReader.readProbabilistic(sequenceFormat));
  }
  
  map<string, string> args(bppoReader.getUnparsedArguments());
  if (verbose)
  {
    ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Sequence format " + suffix, iAln?iAln->getFormatName():iProbAln->getFormatName());
  }

  const Alphabet* alpha2;
  if (AlphabetTools::isRNYAlphabet(alpha))
    alpha2 = &dynamic_cast<const RNY*>(alpha)->getLetterAlphabet();
  else
    alpha2 = alpha;

  AlignedValuesContainer* avc2 = iAln?dynamic_cast<AlignedValuesContainer*>(iAln->readAlignment(sequenceFilePath, alpha2)):dynamic_cast<AlignedValuesContainer*>(iProbAln->readAlignment(sequenceFilePath, alpha2));

  VectorSiteContainer* sites2;
  AlignedValuesContainer* avc;

  try
  {
    sites2=new VectorSiteContainer(*avc2);
  }
  catch (Exception& e)
  {
    sites2=0;
  }

  if (sites2)
  {
    VectorSiteContainer* sites;
    
    /// Look for RNY translation
    if (AlphabetTools::isRNYAlphabet(alpha))
    {
      sites = new VectorSiteContainer(alpha);
      
      const SequenceTools ST;
      for (auto name : sites2->getSequencesNames())
      {
        sites->addSequence(*(ST.RNYslice(sites2->getSequence(name))), false);
      }
      delete sites2;
    }
    else
      sites = sites2;

    // Look for site selection:

    if (iAln->getFormatName() == "MASE file")
    {
      // getting site set:
      string siteSet = ApplicationTools::getStringParameter("siteSelection", args, "none", suffix, suffixIsOptional, warn + 1);
      if (siteSet != "none")
      {
        VectorSiteContainer* selectedSites;
        try
        {
          selectedSites = dynamic_cast<VectorSiteContainer*>(MaseTools::getSelectedSites(*sites, siteSet));
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
        delete sites;
        sites = selectedSites;
      }
    }
    avc=sites;
  }
  else
    avc = new VectorProbabilisticSiteContainer(*avc2); 

  delete avc2;
  
  // getting site set:
  size_t nbSites = avc->getNumberOfSites();
  
  string siteSet = ApplicationTools::getStringParameter("input.site.selection", params, "none", suffix, suffixIsOptional, warn + 1);

  AlignedValuesContainer* selectedSites = 0;
  if (siteSet != "none")
  {
    if(siteSet[0]=='(')
      siteSet=siteSet.substr(1,siteSet.size()-2);
    
    vector<size_t> vSite;
    try
    {
      vector<int> vSite1 = NumCalcApplicationTools::seqFromString(siteSet,",",":");
      
      for (auto ps : vSite1)
      {
        int x = (ps >= 0 ? ps : static_cast<int>(nbSites) + ps + 1);
        if (x<=(int)nbSites)
        {
          if (x > 0)
            vSite.push_back(static_cast<size_t>(x - 1));
          else
            throw Exception("SequenceApplicationTools::getSiteContainer(). Incorrect null index: " + TextTools::toString(x));
        }
        else
          throw Exception("SequenceApplicationTools::getSiteContainer(). Too large index: " + TextTools::toString(x));
      }
      selectedSites = SiteContainerTools::getSelectedSites(*avc, vSite);
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
        
        selectedSites = SiteContainerTools::getSelectedSites(*avc, vSite);
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
    delete avc;
    avc = selectedSites;
  }

  return avc;
}

/******************************************************************************/

AlignedValuesContainer* SequenceApplicationTools::getSitesToAnalyse(
  const AlignedValuesContainer& allSites,
  const map<string, string>& params,
  string suffix,
  bool suffixIsOptional,
  bool gapAsUnknown,
  bool verbose,
  int warn)
{
  // Fully resolved sites, i.e. without jokers and gaps:
  AlignedValuesContainer* sitesToAnalyse;
  AlignedValuesContainer* sitesToAnalyse2;

  const VectorSiteContainer* vsc=dynamic_cast<const VectorSiteContainer*>(&allSites);
  const VectorProbabilisticSiteContainer* vpsc=dynamic_cast<const VectorProbabilisticSiteContainer*>(&allSites);

  if (!vsc && !vpsc)
    throw Exception("SequenceApplicationTools::getSitesToAnalyse: unknown data container.");
  
  size_t numSeq=allSites.getNumberOfSequences();
                                              
  string option = ApplicationTools::getStringParameter("input.sequence.sites_to_use", params, "complete", suffix, suffixIsOptional, warn);
  if (verbose)
    ApplicationTools::displayResult("Sites to use", option);
  if (option == "all")
  {
    sitesToAnalyse = vsc?dynamic_cast<AlignedValuesContainer*>(new VectorSiteContainer(*vsc)):dynamic_cast<AlignedValuesContainer*>(new VectorProbabilisticSiteContainer(*vpsc));

    size_t nbSites=sitesToAnalyse->getNumberOfSites();

    string maxGapOption = ApplicationTools::getStringParameter("input.sequence.max_gap_allowed", params, "100%", suffix, suffixIsOptional, warn);

    double gapCount=0;
    
    if (maxGapOption[maxGapOption.size() - 1] == '%')
    {
      double gapFreq = TextTools::toDouble(maxGapOption.substr(0, maxGapOption.size() - 1))/100;
      gapCount = gapFreq*(int)numSeq;
    }
    else
      gapCount = TextTools::to<int>(maxGapOption)-NumConstants::TINY();
      
    if (gapCount < (double)numSeq  - NumConstants::TINY())
    {
      if (verbose)
        ApplicationTools::displayTask("Remove sites with gaps", true);
      for (size_t i = nbSites; i > 0; i--)
      {
        if (verbose)
          ApplicationTools::displayGauge(nbSites - i, nbSites - 1, '=');

        if ((double)SymbolListTools::numberOfGaps(sitesToAnalyse->getSymbolListSite(i - 1)) > gapCount)
          sitesToAnalyse->deleteSites(i - 1, 1);
      }
      if (verbose)
        ApplicationTools::displayTaskDone();
    }

    string maxUnresolvedOption = ApplicationTools::getStringParameter("input.sequence.max_unresolved_allowed", params, "100%", suffix, suffixIsOptional, warn);

    double unresCount=0;

    if (maxUnresolvedOption[maxUnresolvedOption.size() - 1] == '%')
    {
      double unresFreq = TextTools::toDouble(maxUnresolvedOption.substr(0, maxUnresolvedOption.size() - 1))/100;
      unresCount = unresFreq*(int)numSeq;
    }
    else
      unresCount = TextTools::to<double>(maxUnresolvedOption)-NumConstants::TINY();

    
    if (unresCount < (double)numSeq - NumConstants::TINY())
    {
      if (verbose)
        ApplicationTools::displayTask("Remove unresolved sites", true);
      for (size_t i = nbSites; i > 0; i--)
      {
        if (verbose)
          ApplicationTools::displayGauge(nbSites - i, nbSites - 1, '=');

        if ((double)SymbolListTools::numberOfUnresolved(sitesToAnalyse->getSymbolListSite(i - 1)) > unresCount)
          sitesToAnalyse->deleteSites(i - 1, 1);
      }
      if (verbose)
        ApplicationTools::displayTaskDone();
    }
  }
  else if (option == "complete")
  {
    sitesToAnalyse = SiteContainerTools::getCompleteSites(allSites);
    size_t nbSites = sitesToAnalyse->getNumberOfSites();
    if (verbose)
      ApplicationTools::displayResult("Complete sites", TextTools::toString(nbSites));
  }
  else if (option == "nogap")
  {
    sitesToAnalyse = SiteContainerTools::getSitesWithoutGaps(allSites);
    size_t nbSites = sitesToAnalyse->getNumberOfSites();
    if (verbose)
      ApplicationTools::displayResult("Sites without gap", TextTools::toString(nbSites));
  }
  else
  {
    throw Exception("Option '" + option + "' unknown in parameter 'sequence.sites_to_use'.");
  }

  vsc=dynamic_cast<const VectorSiteContainer*>(sitesToAnalyse);

  const CodonAlphabet* ca = dynamic_cast<const CodonAlphabet*>(sitesToAnalyse->getAlphabet());
  if (vsc && ca)
  {
    option = ApplicationTools::getStringParameter("input.sequence.remove_stop_codons", params, "no", suffix, true, warn);
    if ((option != "") && verbose)
      ApplicationTools::displayResult("Remove Stop Codons", option);

    if (option == "yes")
    {
      string codeDesc = ApplicationTools::getStringParameter("genetic_code", params, "Standard", "", true, warn);
      unique_ptr<GeneticCode> gCode(getGeneticCode(ca->getNucleicAlphabet()->clone(), codeDesc));

      sitesToAnalyse2 = SiteContainerTools::removeStopCodonSites(*vsc, *gCode);
      delete sitesToAnalyse;
    }
    else
      sitesToAnalyse2 = sitesToAnalyse;
  }
  else
    sitesToAnalyse2 = sitesToAnalyse;

  if (verbose)
    ApplicationTools::displayResult("Number of sites", sitesToAnalyse2->getNumberOfSites());

  return sitesToAnalyse2;
}

/******************************************************************************/

void SequenceApplicationTools::writeSequenceFile(
  const SequenceContainer& sequences,
  const map<string, string>& params,
  const string& suffix,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("output.sequence.file", params, true, false, suffix, false, "none", warn);
  string sequenceFormat   = ApplicationTools::getStringParameter("output.sequence.format", params, "Fasta", suffix, false, warn);
  BppOSequenceWriterFormat bppoWriter(warn);
  unique_ptr<OSequence> oSeq(bppoWriter.read(sequenceFormat));
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
  const SiteContainer& sequences,
  const map<string, string>& params,
  const string& suffix,
  bool verbose,
  int warn)
{
  string sequenceFilePath = ApplicationTools::getAFilePath("output.sequence.file", params, true, false, suffix, false, "none", warn);
  string sequenceFormat   = ApplicationTools::getStringParameter("output.sequence.format", params, "Fasta", suffix, false, warn);
  BppOAlignmentWriterFormat bppoWriter(warn);
  unique_ptr<OAlignment> oAln(bppoWriter.read(sequenceFormat));
  if (verbose)
  {
    ApplicationTools::displayResult("Output alignment file " + suffix, sequenceFilePath);
    ApplicationTools::displayResult("Output alignment format " + suffix, oAln->getFormatName());
  }

  // Write sequences:
  oAln->writeAlignment(sequenceFilePath, sequences, true);
}

/******************************************************************************/
