//
// File: SequenceApplicationTools.cpp
// Created by: Julien Dutheil
// Created on: Fri Oct 21 13:13
// from file old ApplicationTools.h created on Sun Dec 14 09:36:26 2003
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "ioseq"

//From Utils:
#include <Utils/ApplicationTools.h>
#include <Utils/TextTools.h>

/******************************************************************************/

Alphabet * SequenceApplicationTools::getAlphabet(
	map<string, string> & params,
	const string & suffix,
	bool suffixIsOptional,
	bool verbose)
{
	Alphabet * chars;
	string alphabet = ApplicationTools::getStringParameter("alphabet", params, "DNA", suffix, suffixIsOptional);
	if(alphabet == "DNA") {
		chars = new DNA();
  	} else if (alphabet == "RNA") {
		chars = new RNA();
  	} else if (alphabet == "Protein") {
		chars = new ProteicAlphabet();
	} else {
		ApplicationTools::error << "Alphabet not known: " << alphabet << endl;
		exit(-1);
	}
	if(verbose) ApplicationTools::displayResult("Alphabet type", alphabet);
	return chars;
}

/******************************************************************************/

VectorSiteContainer * SequenceApplicationTools::getSiteContainer(
	const Alphabet * alpha,
	map<string, string> & params,
	const string & suffix,
	bool suffixIsOptional,
	bool verbose
) {
	string sequenceFilePath = ApplicationTools::getAFilePath("sequence.file",params, true, true, suffix, suffixIsOptional);
	string sequenceFormat = ApplicationTools::getStringParameter("sequence.format", params, "Fasta", suffix, suffixIsOptional);
  	if(verbose) ApplicationTools::displayResult("Sequence format", sequenceFormat);
	ISequence * iSeq = NULL;
	if(sequenceFormat == "Mase") {
		iSeq = new Mase();
	} else if(sequenceFormat == "Phylip")  {
		bool sequential = true, extended = true;
		if(params.find("sequence.format_phylip.order") != params.end()) {
			     if(params["sequence.format_phylip.order"] == "sequential" ) sequential = true;
			else if(params["sequence.format_phylip.order"] == "interleaved") sequential = false;
			else ApplicationTools::displayWarning("Argument '" +
						 params["sequence.format_phylip.order"] +
						 "' for parameter 'sequence.format_phylip.order' is unknown. " +
						 "Default used instead: sequential.");
		} else ApplicationTools::displayWarning("Argument 'sequence.format_phylip.order' not found. Default used instead: sequential.");
		if(params.find("sequence.format_phylip.ext") != params.end()) {
			     if(params["sequence.format_phylip.ext"] == "extended") extended = true;
			else if(params["sequence.format_phylip.ext"] == "classic" ) extended = false;
			else ApplicationTools::displayWarning("Argument '" +
						 params["sequence.format_phylip.ext"] +
						 "' for parameter 'sequence.format_phylip.ext' is unknown. " +
						 "Default used instead: extended.");
		} else ApplicationTools::displayWarning("Argument 'sequence.format_phylip.ext' not found. Default used instead: extended.");
		iSeq = new Phylip(extended, sequential);
	} else if(sequenceFormat == "Fasta") iSeq = new Fasta();
	else if(sequenceFormat == "Clustal") iSeq = new Clustal();
	else {
		ApplicationTools::displayError("Unknown sequence format.");
		exit(-1);
	}
	const SequenceContainer * seqCont = iSeq -> read(sequenceFilePath, alpha);
	VectorSiteContainer * sites = new VectorSiteContainer(* dynamic_cast<const OrderedSequenceContainer *>(seqCont));
  delete seqCont;
	delete iSeq;
	
  if(verbose) ApplicationTools::displayResult("Sequence file " + suffix, sequenceFilePath);

	// Look for site selection:
	if(sequenceFormat == "Mase") {
		//getting site set:
		string siteSet = ApplicationTools::getStringParameter("sequence.format_mase.site_selection", params, "none", suffix, suffixIsOptional, false);
		if(siteSet != "none") {
            VectorSiteContainer * selectedSites;
            try {
                selectedSites = dynamic_cast<VectorSiteContainer *>(MaseTools::getSelectedSites(* sites, siteSet));
                if(verbose) ApplicationTools::displayResult("Set found", TextTools::toString(siteSet) + " sites.");
            } catch(IOException ioe) {
							ApplicationTools::displayError("Site Set '" + siteSet + "' not found.");
				exit(-1);
      }
      if(selectedSites -> getNumberOfSites() == 0) {
				ApplicationTools::displayError("Site Set '" + siteSet + "' is empty.");
				exit(-1);
      }
      delete sites;
      sites = selectedSites;
		}
	}
	return sites;
}

/******************************************************************************/

VectorSiteContainer * SequenceApplicationTools::getSitesToAnalyse(
	const SiteContainer & allSites,
	map<string, string> & params,
	string suffix,
	bool suffixIsOptional,
  bool gapAsUnknown,
	bool verbose)
{
	// Fully resolveid sites, i.e. without jokers and gaps:
	VectorSiteContainer * sitesToAnalyse;
	
	string option = ApplicationTools::getStringParameter("sequence.sites_to_use", params, "complete", suffix, suffixIsOptional);
	if(verbose) ApplicationTools::displayResult("Sites to use", option);
    sitesToAnalyse = new VectorSiteContainer(allSites);
  if(option == "all") {
    if(gapAsUnknown)
    {
      SequenceContainerTools::changeGapsToUnknownCharacters(*sitesToAnalyse);
    }
  } else if(option == "complete") {
		sitesToAnalyse = dynamic_cast<VectorSiteContainer *>(SiteContainerTools::getCompleteSites(allSites));
		int nbSites = sitesToAnalyse -> getNumberOfSites();
		if(verbose) ApplicationTools::displayResult("Complete sites", TextTools::toString(nbSites));
	} else if(option == "nogap") {
		sitesToAnalyse = dynamic_cast<VectorSiteContainer *>(SiteContainerTools::getSitesWithoutGaps(allSites));
		int nbSites = sitesToAnalyse -> getNumberOfSites();
		if(verbose) ApplicationTools::displayResult("Sites without gap", TextTools::toString(nbSites));
	} else {
		ApplicationTools::displayError("Option '" + option + "' unknown in parameter 'sequence.sitestouse'.");
		exit(-1);
	}

	return sitesToAnalyse;
}

/******************************************************************************/

void SequenceApplicationTools::writeSequenceFile(
	const SequenceContainer & sequences,
	map<string, string> & params,
	const string & suffix)
{
	string file   = ApplicationTools::getAFilePath("output.sequence.file", params, true, false, suffix, false);
	string format = ApplicationTools::getStringParameter("output.sequence.format", params, "Fasta", suffix, false, true);
	int ncol      = ApplicationTools::getIntParameter("output.sequence.length", params, 100, suffix, false, false);
	OSequence * oSeq;
	if(format == "Fasta") oSeq = new Fasta(ncol);
	else if(format == "Mase") oSeq = new Mase(ncol);
	else if(format == "Phylip") {
		bool sequential = true, extended = true;
		if(params.find("output.sequence.format_phylip.order" + suffix) != params.end()) {
			     if(params["output.sequence.format_phylip.order"] == "sequential" ) sequential = true;
			else if(params["output.sequence.format_phylip.order"] == "interleaved") sequential = false;
			else ApplicationTools::displayWarning("Argument '" +
						 params["output.sequence.format_phylip.order"] +
						 "' for parameter 'output.sequence.format_phylip.order' is unknown. " +
						 "Default used instead: sequential.");
		} else ApplicationTools::displayWarning("Argument 'output.sequence.format_phylip.order' not found. Default used instead: sequential.");
		if(params.find("output.sequence.format_phylip.ext" + suffix) != params.end()) {
			     if(params["output.sequence.format_phylip.ext"] == "extended") extended = true;
			else if(params["output.sequence.format_phylip.ext"] == "classic" ) extended = false;
			else ApplicationTools::displayWarning("Argument '" +
						 params["output.sequence.format_phylip.ext"] +
						 "' for parameter 'output.sequence_phylip.ext' is unknown. " +
						 "Default used instead: extended.");
		} else ApplicationTools::displayWarning("Argument 'output.sequence_phylip.ext' not found. Default used instead: extended.");
		oSeq = new Phylip(extended, sequential, ncol);
	} else {
		ApplicationTools::displayError("Format '" + format + "' unknown.");
		exit(-1);
	}
	
	// Write sequences:
	oSeq -> write(file, sequences, true);
	
	delete oSeq;
}

/******************************************************************************/

void SequenceApplicationTools::printInputAlignmentHelp()
{
  ApplicationTools::message << "Input sequence file and format:" << endl;
  ApplicationTools::message << "alphabet                      | the alphabet to use [DNA|RNA|Proteins]" << endl;
  ApplicationTools::message << "sequence.format               | [Fasta|Mase|Phylip|Clustal|DCSE]" << endl;
  ApplicationTools::message << "sequence.format_phylip.order  | [interleaved|sequential]" << endl;
  ApplicationTools::message << "sequence.format_phylip.ext    | [classic|extended]" << endl;
  ApplicationTools::message << "sequence.sites_to_use         | [all|nogap|complete]" << endl;
  ApplicationTools::message << "______________________________|___________________________________________" << endl;
}

/******************************************************************************/

void SequenceApplicationTools::printOutputSequenceHelp()
{
	ApplicationTools::message << "Output sequence file and format:" << endl;
  ApplicationTools::message << "output.sequence.format        | [Fasta|Mase|Phylip|Clustal|DCSE]" << endl;
  ApplicationTools::message << "output.sequence.              |" << endl;
  ApplicationTools::message << "           format_phylip.order| [interleaved|sequential]" << endl;
  ApplicationTools::message << "output.sequence.              |" << endl;
  ApplicationTools::message << "             format_phylip.ext| [classic|extended]" << endl;
  ApplicationTools::message << "______________________________|___________________________________________" << endl;
}

/******************************************************************************/

