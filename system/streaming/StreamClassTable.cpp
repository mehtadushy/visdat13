//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StreamClassTable.h"
//---------------------------------------------------------------------------
#include "PAssert.h"
#include "ObjectStreams.h"
#include "Exceptions.h"
#include "LegacyClassNameTranslation.h"
#include "HashTables.hpp"
//---------------------------------------------------------------------------


MetaClass* StreamClassTable::StreamClassTableMetaClass = NULL;

const MetaClass* StreamClassTable::getInstanceClass() const {
   return getClass();
}

Object* StreamClassTable::newInstance() {
   return new StreamClassTable;
}

const MetaClass* StreamClassTable::getClass() {
   if (StreamClassTableMetaClass==NULL) {
      StreamClassTableMetaClass = new MetaClass("StreamClassTable", "no description.",
                                                newInstance, sizeof(StreamClassTable));
   }
   return StreamClassTableMetaClass;
}

void StreamClassTable::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init(parentClass);
   StreamClassTableMetaClass->copyParentProperties();
}

void StreamClassTable::shutdown() {
   delete StreamClassTableMetaClass;
}


// ----


card32 StreamClassTable::getOrCreateClassID(const MetaClass* mc) {
   card32 id;
   if (idByClass.getData(mc, id)) {
      return id;
   } else {
      id = (card32)classTable.size();
      MetaClassEntry me;
      me.mc = mc;
      me.currentStreamIDNumber = id;
      me.mainVersion = mc->getClassVersion();
      classTable.push_back(me);
      idByClass.addElement(mc, id);
      return id;
   }
}

card32 StreamClassTable::getClassID(const MetaClass* mc) const {
   card32 id;
   if (idByClass.getData(mc, id)) {
      return id;
   } else {
      return BAD_CLASS;
   }
}


const MetaClass *StreamClassTable::getClass(card32 id) const {
   if (id < classTable.size()) {
      return classTable[id].mc;
   } else {
      throw PException("StreamClassTable::getClass - unknown class.");
   }
}

card32 StreamClassTable::getNumClassIDs() const {
   return (card32)classTable.size();
}

void StreamClassTable::pushVersion(card32 id, card32 currentVersion) {
   pAssert(id < classTable.size());
   classTable[id].versionStack.push_back(currentVersion);
}

void StreamClassTable::pushVersion(const MetaClass *mc, card32 currentVersion) {
   card32 id = getClassID(mc);
   pAssert(id != BAD_CLASS);
   pushVersion(id, currentVersion);
}

void StreamClassTable::popVersion(card32 id) {
   pAssert(id < classTable.size());
   pAssert(!classTable[id].versionStack.empty());
   classTable[id].versionStack.pop_back();
}

void StreamClassTable::popVersion(const MetaClass *mc) {
   card32 id = getClassID(mc);
   pAssert(id != BAD_CLASS);
   popVersion(id);
}

card32 StreamClassTable::getCurrentVersion(card32 id) const {
   const MetaClassEntry *me = &(classTable[id]);
   if (!me->versionStack.empty()) {
      return me->versionStack.back();
   } else {
      return me->mainVersion;
   }
}

card32 StreamClassTable::getCurrentVersion(const MetaClass *mc) const {
   card32 id = getClassID(mc);
   pAssert(id != BAD_CLASS);
   return getCurrentVersion(id);
}

static void writeClassTableEntry(void* context, const MetaClass* metaClass, card32 id) {
   OutputObjectStream* s = (OutputObjectStream*)context;

   #ifdef DEBUG_STREAMS
   output << id << " " << metaClass->getClassName() << " " << metaClass->getClassVersion();
   output.cr();
   #endif

   s->write<card32>(id+1000);
   s->writeString(metaClass->getClassName());
   s->write<card32>(metaClass->getClassVersion());
}


void StreamClassTable::write(OutputObjectStream *s) const {
   s->write<card32>(idByClass.count());
   const_cast<StreamClassTable*>(this)->idByClass.forEachEntry(s, writeClassTableEntry);
}

void StreamClassTable::read(InputObjectStream *s) {
   card32 classTableCount;
   s->read<card32>(classTableCount);
   classTable.resize(classTableCount);

   for (card32 classNum = 0; classNum != classTableCount; ++classNum) {
      // Read class ID
      card32 classId;
      s->read<card32>(classId);
      pAssert(classId >= 1000);
      classId -= 1000;
      pAssert(classId < classTableCount);

      // Read class name and get pointer to meta class
      string className;
      s->readString(className);

	  /*
	  // Hack to allow for reading of old portable waegele files
	  if ( className == "Feature" ) className = "PWFeature";
	  else if ( className == "Correspondence" ) className = "PWCorrespondence";
	  else if ( className == "CorrespondenceList" ) className = "PWCorrespondenceList";
	  */

      const MetaClass* metaClass = MetaClass::getClassByName(className);
      if (metaClass == NULL) {
         metaClass = LegacyClassNameTranslation::getCurrentClass(className);
         if (metaClass == NULL) {
            throw EIOException(string("StreamClassTable::read() - class in stream with className ") + className + string(" not found."));
         }
      }
      // Read class version
      card32 classVersion;
      s->read<card32>(classVersion);

      MetaClassEntry me;
      me.mc = metaClass;
      me.currentStreamIDNumber = classId;
      me.mainVersion = classVersion;
      classTable[classId] = me;

      idByClass.addElement(metaClass, classId);

      #ifdef DEBUG_STREAMS
      output << classId << " " << className << " " << (IntToStr(classVersion)).c_str();
      output.cr();
      #endif
   }
}


