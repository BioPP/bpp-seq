//
// File: Site.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_SITE_H
#define BPP_SEQ_SITE_H


#include "CoreSite.h"
#include "IntSymbolList.h"
#include "SiteExceptions.h"

namespace bpp
{
/**
 * @brief The site interface.
 *
 * Define specific attributes and methods for sites manipulation.
 * Currently, this interface does not add any additional method to the CoreSite interface, but this may change in the future.
 */
class SiteInterface :
  public virtual CoreSiteInterface,
  public virtual IntSymbolListInterface
{
  public:
    virtual ~SiteInterface() {}

  public:
    SiteInterface* clone() const = 0;

    virtual bool operator==(const SiteInterface& site)
    {
      // Verify that site's size, position and content are equals
      if (site.size() != size())
        return false;
      if (site.getCoordinate() != getCoordinate()) {
        return false;
      } else {
        for (size_t i = 0; i < site.size(); ++i)
        {
          if (site[i] != operator[](i))
            return false;
        }
        return true;
      }
    }

    virtual bool operator<(const SiteInterface& site)
    {
      return site.getCoordinate() < getCoordinate();
    }

};

/**
 * @brief The Site class.
 *
 * Define specific attributes and methods for sites manipulation.
 * It is very similar to the Sequence object (a site is a vertical sequence!),
 * and characters at each position are coded as integers.
 * Sites have a 'position' attribute.
 * This attribute stands for an indice in a an alignment, and may be used as a unique identifier,
 * in the same manner that names identify sequence objects.
 * But for now, we do not allow to construct a Site directly from a string.
 * This should not be a constraint, since you never read sites directly from a file.
 */
class Site :
  public virtual SiteInterface,
  public AbstractCoreSite,
  public IntSymbolList
{
public:
  /**
   * @brief Build a new void Site object with the specified alphabet.
   *
   * @param alphabet The alphabet to use.
   */
  Site(std::shared_ptr<const Alphabet>& alphabet):
      AbstractTemplateSymbolList(alphabet),
      AbstractCoreSite(),
      IntSymbolList(alphabet)
  {}

  /**
   * @brief Build a new void Site object with the specified alphabet and position.
   *
   * @param alphabet   The alphabet to use.
   * @param coordinate The coordinate attribute of this site.
   */
  Site(std::shared_ptr<const Alphabet>& alphabet, int coordinate) :
      AbstractTemplateSymbolList(alphabet),
      AbstractCoreSite(coordinate),
      IntSymbolList(alphabet)
  {}

  /**
   * @brief Build a new Site object with the specified alphabet.
   * The content of the site is initialized from a vector of characters.
   *
   * @param site     The content of the site.
   * @param alphabet The alphabet to use.
   * @throw BadCharException If the content does not match the specified alphabet.
   */
  Site(const std::vector<std::string>& site, std::shared_ptr<const Alphabet>& alphabet):
      AbstractTemplateSymbolList(alphabet),
      AbstractCoreSite(),
      IntSymbolList(site, alphabet)
  {}

  /**
   * @brief Build a new Site object with the specified alphabet and position.
   * The content of the site is initialized from a vector of characters.
   *
   * @param site       The content of the site.
   * @param alphabet   The alphabet to use.
   * @param coordinate The coordinate attribute of this site.
   * @throw BadCharException If the content does not match the specified alphabet.
   */
  Site(const std::vector<std::string>& site, std::shared_ptr<const Alphabet>& alphabet, int coordinate):
      AbstractTemplateSymbolList(alphabet),
      AbstractCoreSite(coordinate),
      IntSymbolList(site, alphabet)
  {}

  /**
   * @brief Build a new Site object with the specified alphabet.
   * The content of the site is initialized from a vector of integers.
   *
   * @param site     The content of the site.
   * @param alphabet The alphabet to use.
   * @throw BadIntException If the content does not match the specified alphabet.
   */
  Site(const std::vector<int>& site, std::shared_ptr<const Alphabet>& alphabet):
      AbstractTemplateSymbolList(site, alphabet),
      AbstractCoreSite(),
      IntSymbolList(site, alphabet)
  {}

  /**
   * @brief Build a new Site object with the specified alphabet and position.
   * The content of the site is initialized from a vector of integers.
   *
   * @param site       The content of the site.
   * @param alphabet   The alphabet to use.
   * @param coordinate The coordinate attribute of this site.
   * @throw BadIntException If the content does not match the specified alphabet.
   */
  Site(const std::vector<int>& site, std::shared_ptr<const Alphabet> alphabet, int coordinate) : 
      AbstractTemplateSymbolList(site, alphabet),
      AbstractCoreSite(coordinate),
      IntSymbolList(site, alphabet)
  {}

  /**
   * @brief The copy constructor.
   */
  Site(const Site& site):
      AbstractTemplateSymbolList(site),
      AbstractCoreSite(site.getCoordinate()),
      IntSymbolList(site)
  {}

  /**
   * @brief The assignment operator.
   */
  Site& operator=(const Site& s)
  {
    AbstractTemplateSymbolList::operator=(s);
    AbstractCoreSite::operator=(s);
    IntSymbolList::operator=(s);
    return *this;
  }

  virtual ~Site() {}

public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  Site* clone() const { return new Site(*this); }
  /** @} */

   double getStateValueAt(size_t sequencePosition, int state) const
   {
     if (sequencePosition  >= size()) throw IndexOutOfBoundsException("Site::getStateValueAt.", sequencePosition, 0, size() - 1);
     return getAlphabet()->isResolvedIn((*this)[sequencePosition], state) ? 1. : 0.;
   }

};

} // end of namespace bpp.

#endif // BPP_SEQ_SITE_H
