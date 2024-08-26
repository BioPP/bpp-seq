// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "Alphabet/AlphabetTools.h"
#include "ProbabilisticSymbolList.h"

using namespace bpp;
using namespace std;

/****************************************************************************************/

ProbabilisticSymbolList::ProbabilisticSymbolList(std::shared_ptr<const Alphabet>& alpha) :
	alphabet_(alpha), content_(alpha->getResolvedChars().size(), 0)
{
	content_.setRowNames(alphabet_->getResolvedChars());
}

ProbabilisticSymbolList::ProbabilisticSymbolList(const DTable& list, std::shared_ptr<const Alphabet>& alpha) :
	alphabet_(alpha), content_(alpha->getResolvedChars().size(), 0)
{
	content_.setRowNames(alpha->getResolvedChars());
	setContent(list);
}

ProbabilisticSymbolList::ProbabilisticSymbolList(const vector<vector<double> >& list, std::shared_ptr<const Alphabet>& alpha) :
	alphabet_(alpha), content_(alpha->getResolvedChars().size(), 0)
{
	content_.setRowNames(alpha->getResolvedChars());
	setContent(list);
}

/****************************************************************************************/

ProbabilisticSymbolList::ProbabilisticSymbolList(const ProbabilisticSymbolListInterface& list) :
	alphabet_(list.getAlphabet()), content_(list.getContent())
{
}

ProbabilisticSymbolList::ProbabilisticSymbolList(const ProbabilisticSymbolList& list) :
	alphabet_(list.alphabet_), content_(list.content_)
{
}

ProbabilisticSymbolList::ProbabilisticSymbolList(const CruxSymbolListInterface& list) :
	alphabet_(list.getAlphabet()), content_(list.getAlphabet()->getResolvedChars().size(), list.size())
{
	size_t nbc = getAlphabet()->getResolvedChars().size();
	content_.setRowNames(alphabet_->getResolvedChars());

	for (size_t i = 0; i < size(); ++i)
	{
		for (size_t s = 0; s < nbc; ++s)
		{
			content_(s, i) = list.getStateValueAt(i, static_cast<int>(s));
		}
	}
}

ProbabilisticSymbolList& ProbabilisticSymbolList::operator=(const ProbabilisticSymbolListInterface& list)
{
	alphabet_ = list.getAlphabet();
	setContent(list.getContent());
	return *this;
}

ProbabilisticSymbolList& ProbabilisticSymbolList::operator=(const ProbabilisticSymbolList& list)
{
	alphabet_ = list.alphabet_;
	content_ = list.content_;
	return *this;
}

/****************************************************************************************/

void ProbabilisticSymbolList::setContent(const DTable& list)
{
	if (list.hasRowNames())
	{
		if (list.getRowNames().size() != alphabet_->getResolvedChars().size())
			throw DimensionException("ProbabilisticSymbolList::setContent. ", list.getRowNames().size(), alphabet_->getResolvedChars().size());

		std::vector<std::string> column_names = list.getRowNames();
		std::vector<std::string> resolved_chars = alphabet_->getResolvedChars();
		for (std::size_t i = 0; i < list.getRowNames().size(); ++i)
		{
			if (column_names[i] != resolved_chars[i])
				throw Exception("ProbabilisticSymbolList::setContent. Row names / resolved characters of alphabet mismatch at " + TextTools::toString(column_names[i]) + " and " + TextTools::toString(resolved_chars[i]) + ".");
		}
	}
	else // DTable has no row names

	{
		if (list.getNumberOfRows() != alphabet_->getResolvedChars().size())
			throw DimensionException("ProbabilisticSymbolList::setContent. ", list.getNumberOfRows(), alphabet_->getResolvedChars().size());
	}

	content_ = list; // final check passes, content_ becomes DTable

	if (!list.hasRowNames())
	{
		content_.setRowNames(alphabet_->getResolvedChars());
	}
}

/****************************************************************************************/

string ProbabilisticSymbolList::toString() const
{
	stringstream ss;
	ss.precision(10);

	for (size_t j = 0; j < content_.getNumberOfColumns(); ++j)
	{
		if (j != 0)
			ss << "|";

		for (size_t i = 0; i < content_.getNumberOfRows(); ++i)
		{
			ss << content_.getRowName(i) << "(";
			ss << content_(i, j) << ")";
		}
	}

	string st;
	ss >> st;
	return st;
}

/****************************************************************************************/

void ProbabilisticSymbolList::setContent(const std::vector<std::vector<double> >& list)
{
	if (list.size() == 0)
		return;

	if (list[0].size() != alphabet_->getResolvedChars().size())
		throw DimensionException("BasicProbabilisticSymbolList::setContent. ", list[0].size(), alphabet_->getResolvedChars().size());

	content_ = list; // final check passes, content_ becomes DTable
	content_.setRowNames(alphabet_->getResolvedChars());
}

/****************************************************************************************/

void ProbabilisticSymbolList::addElement(const std::vector<double>& element)
{
	// now we add this 'row', to the content DTable, padding the end
	// with 0's should its length be smaller than the width of this DTable
	if (element.size() < content_.getNumberOfRows())
	{
		std::vector<double> padded_element(element);

		padded_element.resize(content_.getNumberOfRows(), 0.);

		content_.addColumn(padded_element);
	}
	else
	{
		if (element.size() > content_.getNumberOfRows())
		{
			throw BadSizeException("ProbabilisticSymbolList::addElement: too long element: ",
			                       element.size(),
			                       content_.getNumberOfRows());
		}
		else
			content_.addColumn(element);
	}
}

void ProbabilisticSymbolList::addElement(size_t pos, const std::vector<double>& element)
{
	// now we add this 'row', to the content DTable, padding the end
	// with 0's should its length be smaller than the width of this DTable
	if (element.size() < content_.getNumberOfRows())
	{
		std::vector<double> padded_element(element);

		padded_element.resize(content_.getNumberOfRows(), 0.);
		content_.addColumn(padded_element, (int)pos);
	}
	else
	{
		content_.addColumn(element, (int)pos);
	}
}

void ProbabilisticSymbolList::setElement(size_t pos, const std::vector<double>& element)
{
	if (element.size() < content_.getNumberOfRows())
	{
		std::vector<double> padded_element(element);

		padded_element.resize(content_.getNumberOfRows(), 0.);
		content_.setColumn(padded_element, pos);
	}
	else
	{
		content_.setColumn(element, pos);
	}
}
