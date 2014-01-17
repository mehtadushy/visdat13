//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "CopyObjectProperties.h"
//---------------------------------------------------------------------------
#include "Object.h"
#include "Exceptions.h"
#include "ClassProperty.h"
//---------------------------------------------------------------------------


void copyObjectProperties(const Object *from, Object *to) {
   if (from == NULL || to == NULL) return;
   const MetaClass *fromMC = from->getInstanceClass();
   const MetaClass *toMC = to->getInstanceClass();

   if (!(toMC->inheritsFrom(fromMC))) {
      throw PException("copyObjectProperties - type mismatch");
   }

   const card32 n = fromMC->getNumProperties();
   for (card32 i=0; i<n; i++) {
      fromMC->getProperty(i)->assignProperty(from, to, 0);
   }
}
