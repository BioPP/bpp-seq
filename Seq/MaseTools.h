//
// File: MaseTools.h
// Created by: Julien Dutheil <julien.dutheil@ens-lyon.fr>
// Created on: Tue Apr  1 09:16:59 2003
//

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef _MASETOOLS_H_
#define _MASETOOLS_H_

#include "SequenceContainer.h"
#include "OrderedSequenceContainer.h"
#include "SequenceContainerTools.h"
#include "SiteContainer.h"
#include "SiteContainerTools.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <cstdlib>


class MaseTools
{
	public:
    
        /**
         * @brief Get a site selection from a Mase+ header file.
         *
         * @param maseFileHeader The header of the mase+ file as comments lines.
         * @param setName        The name of the set to retrieve.
         * @throw IOException If the specified set is not found.
         */
		static SiteSelection getSiteSet(const Comments & maseFileHeader, const string & setName) throw (IOException);

        /**
         * @brief Get a sequence selection from a Mase+ header file.
         *
         * @param maseFileHeader The header of the mase+ file as comments lines.
         * @param setName        The name of the set to retrieve.
         * @throw IOException If the specified set is not found.
         */
        static SequenceSelection getSequenceSet(const Comments & maseFileHeader, const string & setName) throw (IOException);

        /**
         * @brief Create a new container corresponding to a site set given in the mase+ format.
         *
         * A new VectorSiteContainer is created, whose destruction is up to the user.
         * The container passed as argument must have 'general comments' in the mase+ format.
         * This function calls the getSiteSet() function on the comments and then calls for
         * SiteContainerTools::getSelectedSites() on the selection.
         *
         * @param sequences The container to get the sites from.
         * @param setName   The name of the set to retrieve.
         * @throw IOException If the specified set is not found.
         */
        static SiteContainer * getSelectedSites(const SiteContainer & sequences, const string & setName) throw (IOException);

        /**
         * @brief Create a new container corresponding to a site set given in the mase+ format.
         *
         * A new VectorSequenceContainer is created, whose destruction is up to the user.
         * The container passed as argument must have 'general comments' in the mase+ format.
         * This function calls the getSequenceSet() function on the comments and then calls for
         * SiteContainerTools::getSelectedSequences() on the selection.
         *
         * @param sequences The container to get the sequence from.
         * @param setName   The name of the set to retrieve.
         * @throw IOException If the specified set is not found.
         */
        static SequenceContainer * getSelectedSequences(const OrderedSequenceContainer & sequences, const string & setName) throw (IOException);

        /**
         * @brief Get a list of all available site selections.
         *
         * @param maseComments Comments as described in the Mase+ format specification.
         * @return A vector of selection names.
         */
        static map<string, unsigned int> getAvailableSiteSelections(const Comments & maseHeader);

        /**
         * @brief Get a list of all available sequences selections.
         *
         * @param maseComments Comments as described in the Mase+ format specification.
         * @return A vector of selection names.
         */
        static map<string, unsigned int> getAvailableSequenceSelections(const Comments & maseHeader);
};


#endif	//_MASETOOLS_H_
