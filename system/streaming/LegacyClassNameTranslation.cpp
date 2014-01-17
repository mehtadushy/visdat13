//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "LegacyClassNameTranslation.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "MetaClass.h"
#include "Persistent.h"
//---------------------------------------------------------------------------



map<string, const MetaClass*> LegacyClassNameTranslation::translations;


void LegacyClassNameTranslation::registerTranslation(string legacyClassName, const MetaClass *currentClass) {
   pAssert(translations.find(legacyClassName) == translations.end());
   pAssert(!MetaClass::isClassRegistered(legacyClassName));
   translations.insert(pair<string, const MetaClass*>(legacyClassName, currentClass));
}

const MetaClass *LegacyClassNameTranslation::getCurrentClass(string legacyClassName) {
   if (translations.find(legacyClassName) == translations.end()) return NULL;
   return translations.find(legacyClassName)->second;
}


