/*
 * File Site.h
 * Author : Julien Dutheil <julien.dutheil@ens-lyon.fr>
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Tuesday August 7 2003
*/

#ifndef _SITE_H_
#define _SITE_H_

#include "SymbolList.h"
#include "SiteExceptions.h"

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
 * This should not be a constraint, since you never read sites directly from a file,
 * do you? You read sequences first...
 */
class Site: public SymbolList 
{	
	protected:
		/**
		 * @brief The position associated to this site.
		 */
		int _position;

	public:
    
		/**
		 * @brief Build a new void Site object with the specified alphabet.
		 *
		 * @param alpha The alphabet to use.
		 */
		Site(const Alphabet * alpha);

		/**
		 * @brief Build a new void Site object with the specified alphabet and position.
		 *
		 * @param alpha    The alphabet to use.
		 * @param position The position attribute for this site.
		 */
		Site(const Alphabet * alpha, int position);

		/**
		 * @brief Build a new Site object with the specified alphabet.
		 * The content of the site is initialized from a vector of characters.
		 *
		 * @param site     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadCharException If the content does not match the specified alphabet.
		 */
		Site(const vector<string> & site, const Alphabet * alpha) throw (BadCharException);

		/**
		 * @brief Build a new Site object with the specified alphabet and position.
		 * The content of the site is initialized from a vector of characters.
		 *
		 * @param site     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @param position The position attribute for this site.
		 * @throw BadCharException If the content does not match the specified alphabet.
		 */
		Site(const vector<string> & site, const Alphabet * alpha, int position) throw (BadCharException);

		/**
		 * @brief Build a new Site object with the specified alphabet.
		 * The content of the site is initialized from a vector of integers.
		 *
		 * @param site     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @throw BadIntException If the content does not match the specified alphabet.
		 */
		Site(const vector<int> & site, const Alphabet * alpha) throw (BadIntException);

		/**
		 * @brief Build a new Site object with the specified alphabet and position.
		 * The content of the site is initialized from a vector of integers.
		 *
		 * @param site     The content of the site.
		 * @param alpha    The alphabet to use.
		 * @param position The position attribute for this site.
		 * @throw BadIntException If the content does not match the specified alphabet.
		 */
		Site(const vector<int> & site, const Alphabet * alpha, int position) throw (BadIntException);

		/**
		 * @brief The copy constructor.
		 */
		Site(const Site & site);

		/**
		 * @brief The assignment operator.
		 */
		Site operator = (const Site & s);

		// Class destructor
		~Site();
	
	public:

		/**
		 * @name The Clonable interface
		 *
		 * @{
		 */
		Clonable * clone() const;
		/** @} */

		/**
		 * @name Setting/getting the position of the site.
		 *
		 * @{
		 */

		/**
		 * @brief Get the position of this site.
		 *
		 * @return This site position.
		 */
		virtual int getPosition() const;

		/**
		 * @brief Set the position of this site.
		 *
		 * @param name The new position of the site.
		 */
		virtual void setPosition(int position);
};

// Sites comparison operators overload
bool operator == (const Site & site1, const Site & site2);
bool operator < (const Site & site1, const Site & site2);

#endif	//_SITE_H_
