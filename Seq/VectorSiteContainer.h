//
// File: VectorSiteContainer.h
// Created by: jdutheil <jdutheil@Deedlit>
// Created on: Mon Oct  6 11:50:40 2003
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

#ifndef _VECTORSITECONTAINER_H_
#define _VECTORSITECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "AbstractSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "OrderedSequenceContainer.h"

// From the NumCalc library:
#include <NumCalc/VectorTools.h>

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

/**
 * This class does the same as AlignedSequenceContainer in the SeqLib package.
 * However, sequences are stored as a vector of sites.
 * This means that sequences are obviously aligned.
 * Methods dealing with sites are in o(N), whereas methods dealing with sequences
 * are in o(N²). This is the contrary of the AlignedSequenceContainer class.
 * Since phylogenetic methods deal wit sites, consider using this class instead
 * of the former one.
 */

class VectorSiteContainer : 
	public virtual AbstractSequenceContainer,   //This container implements the SequenceContainer interface
				                  					        	//and use the AbstractSequenceContainer adapter.
	public virtual SiteContainer			        	//This container is a SiteContainer.
{
	
	protected:
		vector<Site     *> _sites;
		vector<string   *> _names;
		vector<Comments *> _comments; //Sequences comments.
		mutable vector<Sequence *> _sequences; //To store pointer toward sequences retrieves (cf. AlignedSequenceContainer).
	
	public:
		VectorSiteContainer(const vector<const Site *> & vs, const Alphabet * alpha) throw (Exception);
		VectorSiteContainer(unsigned int size, const Alphabet * alpha);
		VectorSiteContainer(const Alphabet * alpha);

		VectorSiteContainer(const      VectorSiteContainer & vsc);
		VectorSiteContainer(const            SiteContainer &  sc);
		VectorSiteContainer(const OrderedSequenceContainer & osc);

		VectorSiteContainer& operator = (const      VectorSiteContainer & vsc);
		VectorSiteContainer& operator = (const            SiteContainer &  sc);
		VectorSiteContainer& operator = (const OrderedSequenceContainer & osc);

		~VectorSiteContainer();

	public:

		/**
		 * @name The Clonable interface.
		 *
		 * @{
		 */
		Clonable * clone() const;
		/** @} */

		/**
		 * @name The SiteContainer interface implementation:
		 *
		 * @{
		 */
		const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException);
		void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception);
		Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
		void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException);
		void         addSite(const Site & site,                         bool checkPosition = true) throw (Exception);
		void         addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
		unsigned int getNumberOfSites() const;
		const vector<int> getPositions() const;
		void reindexSites();
		Vint getSitePositions() const;
		/** @} */

		//Theses methods are implemented for this class:
		
		void setComments(unsigned int sequenceIndex, const Comments & comments) throw (IndexOutOfBoundsException);

		// Method to get a sequence object from sequence container
		const Sequence * getSequence(unsigned int sequenceIndex) const throw (IndexOutOfBoundsException);
		const Sequence * getSequence(const string & name       ) const throw (SequenceNotFoundException);

		// Methods to get position of a sequence in sequence container from his name
		// This method is used by delete and remove methods
		unsigned int getSequencePosition(const string & name) const throw (SequenceNotFoundException);

		void setSequence(const string & name,        const Sequence & sequence, bool checkName) throw (Exception);
		void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName) throw (Exception);

		Sequence * removeSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		Sequence * removeSequence(const string & name       ) throw (SequenceNotFoundException);

		void deleteSequence(unsigned int sequenceIndex) throw (IndexOutOfBoundsException);
		void deleteSequence(const string & name       ) throw (SequenceNotFoundException);

		void addSequence(const Sequence & sequence,                             bool checkName = true) throw (Exception);
		void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

		unsigned int getNumberOfSequences() const;

		vector<string> getSequencesNames() const;

		void setSequencesNames(const vector<string> & names, bool checkNames = true) throw (Exception);

		void clear();

		SequenceContainer * getEmptyContainer() const;
	
		
	protected:
		// Create n void sites:
		void realloc(unsigned int n);
};


#endif	//_VECTORSITECONTAINER_H_
