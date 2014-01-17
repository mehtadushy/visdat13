//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "MetaClass.h"
//---------------------------------------------------------------------------
#include "ClassProperty.h"
#include "ClassMethods.h"
#include "ClassCreator.h"
#include "HashTables.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------


// --- MetaClass


deque<MetaClass*> MetaClass::oldClassRegistration;
MetaClass::MetaClass_ClassRegHashTable MetaClass::classRegistration;

mpcard MetaClass::MetaClass_ClassRegHashTable::hashFunction(string className) const {
   card32 hashValue = 0;
   for (unsigned i = 0; i != className.size(); ++i)
      hashValue = (hashValue * 37) + className[i];
   return hashValue;
}

MetaClass::MetaClass(string className, string classDescription, InstanceCreator newInstanceMethod,
                     card32 instanceSize) {
   this->uniqueIdentifier = 0;
   this->parentClass = NULL;
   this->classVersion = 0;

   this->className = className;
   this->classDescription = classDescription;
   this->newInstanceMethod = newInstanceMethod;
   this->instanceSize = instanceSize;
   guiDescr = new GUIDescriptor();
}

/// Obsolete init method, use init(parentClass, classVersion) instead
void MetaClass::init(card32 uniqueIdentifier, const MetaClass *parentClass,
                     card32 classVersion) {
   this->uniqueIdentifier = uniqueIdentifier;
   this->parentClass = (MetaClass*)parentClass;
   this->classVersion = classVersion;
   pAssert(classVersion >= FIRST_VERSION);

   if (parentClass != NULL) {
      ((MetaClass*)parentClass)->descendants.push_back(this);
   }

   // Obsolete, only for downward compatibility with old unique identifier concept
   if (uniqueIdentifier < NO_UNIQUE_ID) {
      size_t s = oldClassRegistration.size();
      if (s <= uniqueIdentifier) {
         oldClassRegistration.resize((uniqueIdentifier+1)*2, NULL);
      }
      if (oldClassRegistration[uniqueIdentifier] == NULL) {
         oldClassRegistration[uniqueIdentifier] = this;
      } else {
         throw EInvalidState(string("MetaClass::init(): Double unique identifier (Class: ")+className+string(")"));
      }
   }

   MetaClass *forgetIt;
   if (!classRegistration.getData(className, forgetIt)) {
      classRegistration.addElement(className, this);
   } else {
      throw EInvalidState(string("MetaClass::init(): Reregistration of a class with the same class name (Class: ")+className+string(")"));
   }
}

void MetaClass::init(const MetaClass *parentClass, card32 version) {
   init(NO_UNIQUE_ID, parentClass, version);
}


void MetaClass::copyParentProperties() {
   if (parentClass != NULL) {
      vector<ClassProperty*>::iterator pos = parentClass->properties.begin();
      vector<ClassProperty*>::iterator end = parentClass->properties.end();
      while (pos != end) {
         properties.push_back((*pos)->copyClassProperty());
         ++pos;
      }
      vector<ClassMethod*>::iterator mpos = parentClass->methods.begin();
      vector<ClassMethod*>::iterator mend = parentClass->methods.end();
      while (mpos != mend) {
         addMethod((*mpos)->copyMethod());
         ++mpos;
      }
   }

   GUIDescriptor *parentGuiD = parentClass->getGUIDescriptor();
   GUIDescriptor *thisGuiD = getGUIDescriptor();

   card32 ncc = parentGuiD->getNumClassCreators();
   for (card32 i = 0; i < ncc; ++i) {
      thisGuiD->addClassCreator(parentGuiD->getClassCreator(i));
   }

   card32 nce = parentGuiD->getNumClassEditors();
   for (card32 i = 0; i < nce; ++i) {
      const MetaClass *pce = parentGuiD->getClassEditor(i);
      thisGuiD->addClassEditor(pce, pce == parentGuiD->getDefaultClassEditor());
   }
}

MetaClass::~MetaClass() {
   {
      vector<ClassProperty*>::iterator pos = properties.begin();
      vector<ClassProperty*>::iterator end = properties.end();
      while (pos != end) {
         delete *pos;
         ++pos;
      }
   }
   {
      vector<ClassMethod*>::iterator pos = methods.begin();
      vector<ClassMethod*>::iterator end = methods.end();
      while (pos != end) {
         delete *pos;
         ++pos;
      }
   }

   delete guiDescr;
/*
   #if defined(_DEBUG) 
   MetaClass *forgetIt;
   if (!classRegistration.getData(className, forgetIt)) {
      string msg = string("MetaClass::~MetaClass() - class is not registered, className: ")+className;
      Application->MessageBox(msg.c_str(), "Warning:",
                              MB_OK | MB_ICONEXCLAMATION);
   }
   #endif*/
   classRegistration.deleteElement(className);
}

void MetaClass::checkAllShutdown() {
  /* #if defined(_DEBUG) 
   if (classRegistration.count() > 0) {
      AnsiString badGuys;
      max_platform_card n = classRegistration.count();
      for (max_platform_card i=0; i<n; i++) {
         badGuys += classRegistration.getKey(i).c_str();
         if (i != n) {
            badGuys += " ";
         }
      }
      AnsiString msg = "Not all MetaClasses have been shutdown properly. Missing: " + badGuys;
      Application->MessageBox(msg.c_str(), "Warning:", MB_ICONEXCLAMATION | MB_OK);
   }
   #endif
	*/
}

Object* MetaClass::newInstance() const {
   if (newInstanceMethod == NULL) {
      throw EClassAbstract("MetaClass::newInstance() - cannot create instance, class is abstract.");
   } else {
      return newInstanceMethod();
   }
}

// obsolete
card32 MetaClass::getUniqueIdentifier() const {
   if (uniqueIdentifier < NO_UNIQUE_ID)
     return uniqueIdentifier;
   else
     throw EInvalidState("MetaClass::getUniqueIdentifier: Method is obsolete");
}

const std::string MetaClass::getClassName() const {
   return className;
}

const std::string MetaClass::getClassDescription() const {
   return classDescription;
}

card32 MetaClass::getClassVersion() const {
  return this->classVersion;
}

bool MetaClass::isAbstract() const {
   return newInstanceMethod == NULL;
}

card32 MetaClass::getInstanceSize() const {
   return instanceSize;
}

bool MetaClass::inheritsFrom(const MetaClass* mc) const {
   if (mc == NULL) return false;
   const MetaClass *currentClass = this;
   do {
      if (currentClass == mc) {
         return true;
      } else {
         currentClass = currentClass->getParentClass();
      }
   } while (currentClass != NULL);
   return false;
}

void MetaClass::addProperty(ClassProperty *prop) {
	pAssert(prop->getIntroducedInVersion() <= classVersion);
   properties.push_back(prop);
   prop->setOwner(this);
}

card32 MetaClass::getNumProperties() const {
   return (card32)properties.size();
}

const ClassProperty *MetaClass::getProperty(card32 i) const {
   return properties[i];
}

card32 MetaClass::getNumMethods() const {
   return (card32)methods.size();
}

void MetaClass::addMethod(ClassMethod *m) {
   methods.push_back(m);
}

const ClassMethod *MetaClass::getMethod(card32 i) const {
   return methods[i];
}

// obsolete
const MetaClass* MetaClass::getClassByUniqueIdentifier(card32 uniqueIdentifier) {
   card32 s = (card32)oldClassRegistration.size();
   if (uniqueIdentifier < s) {
      return oldClassRegistration[uniqueIdentifier];
   } else {
      throw ERangeCheck("MetaClass::getClassByUniqueIdentifier - identifier out of range.");
   }
}

const MetaClass* MetaClass::getClassByName(const string className) {
   MetaClass* metaClass;
   bool success = classRegistration.getData(className, metaClass);

   if (success)
      return metaClass;
   else
      return NULL;
      //throw ERangeCheck("MetaClass::getClassByName - invalid class name");
}

bool MetaClass::isClassRegistered(const string className) {
   MetaClass* forgetIt;
   return classRegistration.getData(className, forgetIt);
}


bool MetaClass::callNoArgsMethod(Object *obj, string methodName) const {
	mpcard n = methods.size();
	for (mpcard i=0; i<n; i++) {
		if (methods[i]->getName() == methodName) {
			NoArgsClassMethod *m = dynamic_cast<NoArgsClassMethod*>(methods[i]);
			if (m != NULL) {
				m->call(obj);
				return true;
			}
		}
	}
	return false;
}


void MetaClass::actProperty_setUserEdit( bool b )
{
	pAssert( properties.size() > 0 );
	properties.back()->setUserEdit( b );
}

void MetaClass::actProperty_setEditReadonly( bool b )
{
	pAssert( properties.size() > 0 );
	properties.back()->setEditReadonly( b );
}

void MetaClass::actProperty_setStreamProperty( bool b )
{
	pAssert( properties.size() > 0 );
	properties.back()->setStreamProperty( b );
}

void MetaClass::actProperty_setPropertyName( string name )
{
	pAssert( properties.size() > 0 );
	properties.back()->setCName( name );
}

void MetaClass::actProperty_addPropertyEditor( const MetaClass *mc, bool defaultEd)
{
	pAssert( properties.size() > 0 );
	properties.back()->addPropertyEditor(mc, defaultEd);
}


// --- GUIDescriptor


GUIDescriptor::GUIDescriptor() {
   autoApplyEditChanges = true;
   icon = ":/foundation/Images/Classes/Object.png";
}

bool GUIDescriptor::getAutoApplyEditChanges() const {
   return autoApplyEditChanges;
}

void GUIDescriptor::setAutoApplyEditChanges(bool aap) {
   this->autoApplyEditChanges = aap;
}

void GUIDescriptor::addClassEditor(const MetaClass *classEditor, bool defaultEditor) {
   if (defaultEditor) {
      classEditors.push_front(classEditor);
   } else {
      classEditors.push_back(classEditor);
   }
}

void GUIDescriptor::addClassCreator(const MetaClass *classCreator) {
   classCreators.push_back(classCreator);
}

void GUIDescriptor::addApplication(ApplicationPtr app) {
   applications.push_back(app);
}


const MetaClass *GUIDescriptor::getDefaultClassCreator() const {
   if (classCreators.empty()) {
      return NULL;
   } else {
      return classCreators[0];
   }
}

const MetaClass *GUIDescriptor::getDefaultClassEditor() const {
   if (classEditors.empty()) {
      return NULL;
   } else {
      return classEditors[0];
   }
}

card32 GUIDescriptor::getNumClassCreators() const {
   return (card32)classCreators.size();
}

card32 GUIDescriptor::getNumClassEditors() const {
   return (card32)classEditors.size();
}

card32 GUIDescriptor::getNumApplications() const {
   return (card32)applications.size();
}


const MetaClass *GUIDescriptor::getClassCreator(card32 i) const {
   return classCreators[i];
}

const MetaClass *GUIDescriptor::getClassEditor(card32 i) const {
   return classEditors[i];
}

GUIDescriptor::ApplicationPtr GUIDescriptor::getApplication(card32 i) const {
   return applications[i];
}

string GUIDescriptor::getIcon() const {
   return icon;
}

void GUIDescriptor::setIcon(string resourceString) {
   this->icon = resourceString;
}
