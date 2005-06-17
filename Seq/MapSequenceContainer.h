//
// File MapSequenceContainer.h
// Authors : Guillaume Deuchst <GDeuchst@ifrance.com>
//           Sylvain Gaillard <yragael2001@yahoo.fr>
// Last modification : Friday June 25 2004
//

/*
Copyright ou © ou Copr. CNRS, (17 Novembre 2004) 

Julien.Dutheil@univ-montp2.fr

Ce logiciel est un programme informatique servant à fournir des classes
pour l'analyse de séquences.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

Julien.Dutheil@univ-montp2.fr

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

#ifndef _MAPSEQUENCECONTAINER_H_
#define _MAPSEQUENCECONTAINER_H_

#include "Sequence.h"
#include "AbstractSequenceContainer.h"
#include "Alphabet.h"

#include <string>
#include <map>

using namespace std;

/**
 * @brief MapSequenceContainer class
 * 
 * Host sequence container's alphabet
 * Define specific methods to map sequences containers manipulation
 *
 * Note : although a sequence can be deleted directly from sequence container,
 * using specific methods (removeSequence and deleteSequence) to do it is IMPERATIVE (because they do
 * verifications, and they properly delete pointer toward sequence)
 *
 * This class can be instanciated (i.e. SequenceContainer sc * = new MapSequenceContainer(seqMap, alphabet);)
 */

class MapSequenceContainer: public virtual AbstractSequenceContainer
{
	protected:
		map<string, Sequence *> _sequences;
        
	public:
		// Class constructors
		MapSequenceContainer(const map<string, Sequence *> & ms, const Alphabet * alpha);
		MapSequenceContainer(const Alphabet * alpha);

		// Copy constructors
		MapSequenceContainer(const MapSequenceContainer & msc);
		MapSequenceContainer & operator = (const MapSequenceContainer & msc);

		// Class destructor
		~MapSequenceContainer();

	public:
		/**
		 * @brief Get a sequence.
		 */
		const Sequence * getSequenceByKey(const string & key)  const throw (SequenceNotFoundException);

		/**
		 * @brief Set a sequence.
		 */
		void setSequenceByKey(const string & key , const Sequence & sequence, bool checkNames = true) throw (SequenceNotFoundException);

		/**
		 * @brief Remove a sequence.
		 */
		Sequence * removeSequenceByKey(const string & key);

		/**
		 * @brief Delete a sequence.
		 */
		void deleteSequenceByKey(const string & key);

		/**
		 * @brief Add a sequence.
		 */
		void addSequence(const string & key, const Sequence & sequence, bool checkNames = true) throw (Exception);

		/**
		 * @brief Get the sequences keys.
		 */
		vector<string> getKeys() const;

		/**
		 * @brief Get a key at a position.
		 */
		string getKey(unsigned int pos) const throw (IndexOutOfBoundsException);

		/**
		 * @brief Get a sequence key.
		 */
		string getKey(const string & name) const throw (SequenceNotFoundException);

		/**
		 * @name The clonable interface
		 * @{
		 */
		Clonable * clone() const;
		/**
		 * @}
		 */

		/**
		 * @name The SequenceContainer interface implementation:
		 *
		 * @{
		 */
		const Sequence * getSequence(const string & name) const throw (SequenceNotFoundException);
		void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (SequenceNotFoundException);
		Sequence * removeSequence(const string & name) throw (SequenceNotFoundException);
		void deleteSequence(const string & name) throw (SequenceNotFoundException);
		unsigned int getNumberOfSequences() const;
		void clear();
		SequenceContainer * getEmptyContainer() const;
		/** @} */

		/**
		 * @name The OrderedSequenceContainer interface implementation:
		 *
		 * @{
		 */
		const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		unsigned int     getSequencePosition(const string & name) const throw (SequenceNotFoundException);
		void             setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (IndexOutOfBoundsException);
		Sequence *    removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void          deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);
		vector<string> getSequencesNames() const;
		void setSequencesNames(const vector<string> & names, bool checkNames) throw (Exception);
		/** @} */

		/**
		 * @name AbstractSequenceContainer methods.
		 *
		 * @{
		 */
		Sequence * getSequence(unsigned int i)      throw (IndexOutOfBoundsException);
		Sequence * getSequence(const string & name) throw (SequenceNotFoundException);
		/** @} */
};

#endif // _MAPSEQUENCECONTAINER_H_
