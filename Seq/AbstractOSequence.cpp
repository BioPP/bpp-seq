
#include "AbstractOSequence.h"

// From the STL:
#include <fstream>
using namespace std;

void AbstractOSequence::write(const string & path, const SequenceContainer & sc, bool overwrite) const throw (Exception)
{
	// Open file in specified mode
	ofstream output(path.c_str(), overwrite ? (ios::out) : (ios::out|ios::app));
	write(output, sc);
	output.close();
}
