
#include "AbstractISequence.h"

// From the STL:
#include <fstream>
using namespace std;

void AbstractISequence::read(const string & path , VectorSequenceContainer & sc) const throw (Exception)
{
	ifstream input(path.c_str(), ios::in);
	read(input, sc);
	input.close();
}

VectorSequenceContainer * AbstractISequence::read(istream & input, const Alphabet * alpha) const throw (Exception)
{
	VectorSequenceContainer * vsc = new VectorSequenceContainer(alpha);
	read(input, *vsc);
	return vsc;
}
VectorSequenceContainer * AbstractISequence::read(const string & path, const Alphabet * alpha) const throw (Exception)
{
	VectorSequenceContainer * vsc = new VectorSequenceContainer(alpha);
	read(path, *vsc);
	return vsc;
}
