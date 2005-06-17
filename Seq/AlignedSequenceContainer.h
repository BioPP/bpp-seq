//
// File AlignedSequenceContainer.h
// Created by: Guillaume Deuchst
// //             Julien Dutheil
// Last modification : Friday August 22 2003
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

#ifndef _ALIGNEDSEQUENCECONTAINER_H_
#define _ALIGNEDSEQUENCECONTAINER_H_

#include "Site.h"
#include "SiteContainer.h"
#include "OrderedSequenceContainer.h"
#include "VectorSequenceContainer.h"

// From Utils:
#include <Utils/Exceptions.h>

/* AlignedSequenceContainer class
 * Define specific methods to sites containers manipulation
 * 
 * This class can be instanciated (i.e. AlignedSequenceContainer asc * = new AlignedSequenceContainer(vsc);)
*/

class AlignedSequenceContainer : public virtual VectorSequenceContainer, public virtual SiteContainer
{
	protected:
		// Integer vector that contains sites's positions
		vector<int> positions;
		
		unsigned int length; // Number of sites for verifications before sequence's insertion in sequence container
	
		/* This is used in order to implement the SiteContainer interface.
		 * A SiteContainer is expected to work on Site objects, but this class
		 * -- since it is a VectorSequenceContainer -- has its data sored as
		 * Sequence object. When the SiteContainer method getSite() is invoked
		 * it creates a new Site object and send the adress of it.
		 * To avoid memory leaks, this object is put into a vector so that it can be
		 * destroyed when the container is destroyed.
		 */
		mutable vector<Site *> sites;
		
	public:
		// Class constructors
		AlignedSequenceContainer(const Alphabet * alpha);
		AlignedSequenceContainer(const AlignedSequenceContainer & asc);
		AlignedSequenceContainer(const            SiteContainer &  sc);
		AlignedSequenceContainer(const OrderedSequenceContainer & osc);

		AlignedSequenceContainer & operator = (const AlignedSequenceContainer & asc);
		AlignedSequenceContainer & operator = (const            SiteContainer &  sc);
		AlignedSequenceContainer & operator = (const OrderedSequenceContainer & osc) throw (SequenceNotAlignedException);

		// Class destructor
		~AlignedSequenceContainer();

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
  	const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException) ;
		void         setSite(unsigned int siteIndex, const Site & site, bool checkPosition = true) throw (Exception) ;
		Site *    removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
		void      deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) ;
		void addSite(const Site & site, bool checkPosition = true) throw (Exception);
		void addSite(const Site & site, unsigned int siteIndex, bool checkPosition = true) throw (Exception);
		unsigned int getNumberOfSites() const;
		Vint getSitePositions() const;
		void reindexSites();
		/** @} */

		// Method to replace a sequence in sequence container
		void setSequence(const string & name, const Sequence & sequence, bool checkName = true) throw (Exception);
		void setSequence(unsigned int sequenceIndex, const Sequence & sequence, bool checkName = true) throw (Exception);

		// Methods to add sequence in sequence container
		// CheckName : boolean for enable or disable sequence's name existence checking
		void addSequence(const Sequence & sequence, bool checkName = true) throw (Exception);
		void addSequence(const Sequence & sequence, unsigned int sequenceIndex, bool checkName = true) throw (Exception);

		// Method to delete all sequences in sequence container
		void clear();
		
		SequenceContainer * getEmptyContainer() const;
	
	protected:
		// Protected method to verify sequence's size before insertion in sequence container
		bool checkSize(const Sequence & sequence);
};

#endif // _ALIGNEDSEQUENCECONTAINER_H_
