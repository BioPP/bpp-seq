
#include "AbstractISequence.h"

VectorSequenceContainer * AbstractISequence::read(const string & path, const Alphabet * alpha) const throw (Exception)
{
	VectorSequenceContainer * vsc = new VectorSequenceContainer(alpha);
	read(path, *vsc);
	return vsc;
}
