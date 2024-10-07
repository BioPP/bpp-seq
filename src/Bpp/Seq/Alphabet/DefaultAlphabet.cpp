// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Text/TextTools.h>

#include "AlphabetState.h"
#include "DefaultAlphabet.h"

using namespace bpp;

DefaultAlphabet::DefaultAlphabet() :
  chars_("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.?")
{
  // Alphabet content definition
  registerState(new AlphabetState(-1, "-", "Gap"));

  for (size_t i = 0; i < chars_.size(); i++)
  {
    registerState(new AlphabetState(static_cast<int>(i), TextTools::toString(chars_[i]), ""));
  }
}
