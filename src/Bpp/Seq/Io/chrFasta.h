
#ifndef _CHRFASTA_H_
#define _CHRFASTA_H_

#include <Bpp/Seq/Alphabet/Alphabet.h>
#include <Bpp/Seq/Alphabet/ChromosomeAlphabet.h>
#include "AbstractISequence.h"
#include "AbstractIAlignment.h"
#include "AbstractOSequence.h"
#include "../Sequence.h"
#include "../Container/SequenceContainer.h"
#include "../Container/VectorSequenceContainer.h"
#include "ISequenceStream.h"
#include "OSequenceStream.h"
#include "SequenceFileIndex.h"


namespace bpp{


/**
 * @brief The fasta sequence file format.
 *
 * Read and write from/to Fasta files.
 */
class chrFasta
{

  public:
    static VectorSequenceContainer* readSequencesFromFile(const std::string& path , ChromosomeAlphabet* alpha);
    static bool nextSequence(std::istream& input, Sequence& seq, ChromosomeAlphabet* alpha);
    virtual ~chrFasta() {}


};

}

#endif // _CHRFASTA_H_