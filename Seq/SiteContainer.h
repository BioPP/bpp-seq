//
// File SiteContainer.h
// Created by: Guillaume Deuchst <GDeuchst@ifrance.com>
//             Julien Dutheil <julien.dutheil@ens-lyon.fr>
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

#ifndef _SITECONTAINER_H_
#define _SITECONTAINER_H_

#include "Site.h"
#include "OrderedSequenceContainer.h"
#include "SequenceContainerExceptions.h"

// From Utils:
#include <Utils/Exceptions.h>

// From NumCalc:
#include <NumCalc/VectorTools.h>

// From the STL:
#include <cstdlib>

using namespace std;

/**
 * @brief The SiteContainer interface.
 *
 * Container implementing the SiteContainer interface deal with <em>aligned</em> sequences.
 * This interface provides methods to retrieve, add or set sites in the alignment.
 * As for SequenceContainers, the maintenance of Sites is up to the container.
 * All site objects are cloned befored being added and retrieved.
 * All sites stored are deleted in the destructor of the container or after having called the deleteSite() method.
 */

class SiteContainer: public virtual OrderedSequenceContainer
{
	public:
		virtual ~SiteContainer() {}

	public:
	
		/**
		 * @brief Get a site from the container.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @return A site objet corresponding to site i in the alignment.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual const Site * getSite(unsigned int siteIndex) const throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Set a site in the container.
		 *
		 * @param siteIndex     The position of the site in the container.
		 * @param site          The site to set.
		 * @param checkPosition Look if the position of the new site match a position attribute in the container.
		 * @throw Exception If the specified site does not exists or is not correct.
		 */
		virtual void setSite(unsigned int siteIndex, const Site & site, bool checkPosition) throw (Exception) = 0;

		/**
		 * @brief Remove a site from the container.
		 *
		 * The site is not deleted, a pointer toward it is returned.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @return A pointer toward site i in the alignment.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual Site * removeSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Delete a site in the container.
		 *
		 * @param siteIndex The position of the site in the container.
		 * @throw IndexOutOfBoundsException If the specified site does not exists.
		 */
		virtual void deleteSite(unsigned int siteIndex) throw (IndexOutOfBoundsException) = 0;

		/**
		 * @brief Get the number of sites in the container.
		 *
		 * @return The number of sites in the container.
		 */
		virtual unsigned int getNumberOfSites() const = 0;

		/**
		 * @brief Set all positions attributes.
		 */
		virtual void reindexSites() = 0;

		/**
		 * @brief Get all position attributes of sites.
		 *
		 * @return A vector with all site positions.
		 */
		virtual Vint getSitePositions() const = 0;
};

#endif	//_SITECONTAINER_H_
