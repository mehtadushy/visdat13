//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "Persistent.h"
//---------------------------------------------------------------------------
#include "ClassProperty.h"
#include "ObjectStreams.h"
#include "Tools.h"
//---------------------------------------------------------------------------
#undef DEBUG_STREAMS
#ifdef DEBUG_STREAMS
  #include "GeoXOutput.h"
#endif
//---------------------------------------------------------------------------


MetaClass* Persistent::persistentMetaClass = NULL;

const MetaClass* Persistent::getInstanceClass() const {
   return getClass();
}

const MetaClass* Persistent::getClass() {
   if (persistentMetaClass==NULL) {
      persistentMetaClass = new MetaClass("Persistent", "Base Class for all serializable classes.",
                                          NULL, sizeof(Persistent));
   }
   return persistentMetaClass;
}

void Persistent::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   persistentMetaClass->copyParentProperties();
}

void Persistent::shutdown() {
   delete persistentMetaClass;
}

void Persistent::write(OutputObjectStream *s) const {
   writeObjectProperties(s, this);
}

void Persistent::read(InputObjectStream *s) {
   readObjectProperties(s, this);
}

void writeObjectProperties(OutputObjectStream *s, const Persistent *obj) {
   const MetaClass *mc = obj->getInstanceClass();
   #ifdef DEBUG_STREAMS
   output << "**** " << mc->getClassName(); output.cr();
   #endif
   card32 n = mc->getNumProperties();
   for (card32 i=0; i<n; ++i) {
      const ClassProperty *cp = mc->getProperty(i);
      if (cp->getStreamProperty()) {
         #ifdef DEBUG_STREAMS
         output << cp->getTypeName() << " : " << cp->getCName(); output.cr();
         #endif
         cp->writeProperty(obj, NULL, s);
      }
   }
}

void readObjectProperties(InputObjectStream *s, Persistent *obj) {
   const MetaClass *mc = obj->getInstanceClass();
   #ifdef DEBUG_STREAMS
   output << "**** " << mc->getClassName(); output.cr();
   #endif
   card32 n = mc->getNumProperties();
   for (card32 i=0; i<n; ++i) {
      const ClassProperty *cp = mc->getProperty(i);
      if (cp->getStreamProperty()) {
         bool readProperty = false;
         if (cp->getIntroducedInVersion() < 1000) {
            // Stream version
            readProperty = (cp->getIntroducedInVersion() <= s->getVersion());
         } else {
            // Class version
            readProperty = (cp->getIntroducedInVersion() <= s->getClassVersion(cp->getOwner()));
         }

         if (readProperty) {
            #ifdef DEBUG_STREAMS
            output << cp->getTypeName() << " : " << cp->getCName(); output.cr();
            #endif
            cp->readProperty(obj, NULL, s);
         }
      }
   }
}




