// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "Clustal.h"
#include "Dcse.h"
#include "Fasta.h"
#include "GenBank.h"
#include "IoSequenceFactory.h"
#include "Mase.h"
#include "NexusIoSequence.h"
#include "Phylip.h"

using namespace bpp;
using namespace std;

const string IoSequenceFactory::FASTA_FORMAT              = "Fasta";
const string IoSequenceFactory::MASE_FORMAT               = "Mase";
const string IoSequenceFactory::CLUSTAL_FORMAT            = "Clustal";
const string IoSequenceFactory::DCSE_FORMAT               = "DCSE";
const string IoSequenceFactory::PHYLIP_FORMAT_INTERLEAVED = "Phylip I";
const string IoSequenceFactory::PHYLIP_FORMAT_SEQUENTIAL  = "Phylip S";
const string IoSequenceFactory::PAML_FORMAT_INTERLEAVED   = "PAML I";
const string IoSequenceFactory::PAML_FORMAT_SEQUENTIAL    = "PAML S";
const string IoSequenceFactory::GENBANK_FORMAT            = "GenBank";
const string IoSequenceFactory::NEXUS_FORMAT              = "Nexus";

unique_ptr<ISequence> IoSequenceFactory::createReader(const string& format)
{
	if (format == FASTA_FORMAT)
		return make_unique<Fasta>();
	else if (format == MASE_FORMAT)
		return make_unique<Mase>();
	else if (format == CLUSTAL_FORMAT)
		return make_unique<Clustal>();
	else if (format == DCSE_FORMAT)
		return make_unique<DCSE>();
	else if (format == PHYLIP_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(false, false);
	else if (format == PHYLIP_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(false, true);
	else if (format == PAML_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(true, false);
	else if (format == PAML_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(true, true);
	else if (format == GENBANK_FORMAT)
		return make_unique<GenBank>();
	else if (format == NEXUS_FORMAT)
		return make_unique<NexusIOSequence>();
	else
		throw Exception("Format " + format + " is not supported for sequences input.");
}

unique_ptr<IAlignment> IoSequenceFactory::createAlignmentReader(const string& format)
{
	if (format == FASTA_FORMAT)
		return make_unique<Fasta>();
	else if (format == MASE_FORMAT)
		return make_unique<Mase>();
	else if (format == CLUSTAL_FORMAT)
		return make_unique<Clustal>();
	else if (format == DCSE_FORMAT)
		return make_unique<DCSE>();
	else if (format == PHYLIP_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(false, false);
	else if (format == PHYLIP_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(false, true);
	else if (format == PAML_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(true, false);
	else if (format == PAML_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(true, true);
	else if (format == NEXUS_FORMAT)
		return make_unique<NexusIOSequence>();
	else
		throw Exception("Format " + format + " is not supported for alignment input.");
}

unique_ptr<OSequence> IoSequenceFactory::createWriter(const string& format)
{
	if (format == FASTA_FORMAT)
		return make_unique<Fasta>();
	else if (format == MASE_FORMAT)
		return make_unique<Mase>();
	else
		throw Exception("Format " + format + " is not supported for output.");
}

unique_ptr<OAlignment> IoSequenceFactory::createAlignmentWriter(const string& format)
{
	if (format == FASTA_FORMAT)
		return make_unique<Fasta>();
	else if (format == MASE_FORMAT)
		return make_unique<Mase>();
	else if (format == PHYLIP_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(false, false);
	else if (format == PHYLIP_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(false, true);
	else if (format == PAML_FORMAT_INTERLEAVED)
		return make_unique<Phylip>(true, false);
	else if (format == PAML_FORMAT_SEQUENTIAL)
		return make_unique<Phylip>(true, true);
	else
		throw Exception("Format " + format + " is not supported for output.");
}
