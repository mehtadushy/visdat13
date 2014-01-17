//---------------------------------------------------------------------------
#ifndef MetaClassH
#define MetaClassH
//---------------------------------------------------------------------------
#include <string>
#include <list>
#include <vector>
#include <deque>
#include "Exceptions.h"
#include "PTypes.h"
#include "HashTables.h"
//---------------------------------------------------------------------------


using namespace std;

class  Object;
class  ClassProperty;
class  ClassMethod;
class  MetaClass;



class  GUIDescriptor {
 public:
   /// pointer to a procedure that starts a new application that works with the given object
   class ApplicationPtr {
    public:
      typedef void (*ApplicationStartProc)(Object *object);
      ApplicationStartProc startProc;
      string name;
      ApplicationPtr(ApplicationStartProc startProc, string name) {
         this->startProc = startProc; this->name = name;
      }
   };

 private:
   bool autoApplyEditChanges;
   vector<const MetaClass*> classCreators; /* class of ClassCreator */
   deque<const MetaClass*> classEditors; /* class of ClassEditor */
   vector<ApplicationPtr> applications;
   /// resource identifier for icon
   string icon; 

 public:
   GUIDescriptor();
   bool getAutoApplyEditChanges() const; // default = true
   void setAutoApplyEditChanges(bool aap);
   void addClassEditor(const MetaClass *classEditor, bool defaultEditor = false);
   void addClassCreator(const MetaClass *classCreator);
   void addApplication(ApplicationPtr app);
   const MetaClass *getDefaultClassCreator() const; // may return NULL
   const MetaClass *getDefaultClassEditor() const; // default = property browser
   card32 getNumClassCreators() const;
   card32 getNumClassEditors() const;
   card32 getNumApplications() const;
   const MetaClass *getClassCreator(card32 i) const;
   const MetaClass *getClassEditor(card32 i) const;
   ApplicationPtr getApplication(card32 i) const;
   string getIcon() const;
   void setIcon(string resourceString);
};



class  MetaClass {
 private:

   class  MetaClass_ClassRegHashComparator {
     public:
       static bool equals(string obj1, string obj2) { return obj1 == obj2; }
   };

   class  MetaClass_ClassRegHashTable : public HashTable<string, MetaClass*, MetaClass_ClassRegHashComparator> {
    public:
      virtual max_platform_card hashFunction(string className) const;
   };

   typedef Object* (*InstanceCreator)();

   static deque<MetaClass*> oldClassRegistration;  // obsolete, legacy data structure
   static MetaClass_ClassRegHashTable classRegistration;

   card32 uniqueIdentifier;  // obsolete, >= 1000 means: not used
   card32 instanceSize;
   string className;
   string classDescription;
   card32 classVersion;      // has to be >= 1000
   InstanceCreator newInstanceMethod;
   MetaClass *parentClass;
   GUIDescriptor *guiDescr;

   vector<ClassProperty*> properties;
   vector<ClassMethod*> methods;
   vector<MetaClass*> descendants;

 public:
   class  EClassAbstract : public PException {
    public:
      EClassAbstract(string msg) : PException(msg) {}
	   virtual string getTypeString() {return "EClassAbstract";}
   };

   typedef card32 ClassVersion;

   static const ClassVersion FIRST_VERSION = 1000;

   static const card32 NO_UNIQUE_ID = 1000; /// end of legacy class id range

   /** create new meta class. newInstanceMethod == NULL for abstract classes. */
   MetaClass(string className, string classDescription, InstanceCreator newInstanceMethod,
             card32 instanceSize);
   /// initialize meta class.
   /// parentClass - parent class, NULL for root class ("Object")
   /// version -  per class version number, set to FIRST_VERSION + number
   void         init(const MetaClass *parentClass, card32 classVersion = FIRST_VERSION);

   /// ---legacy--- initialize meta class (legacy method, do not use for new code).
   /// ---legacy--- uniqueIdentifier - unique class id (for new files: automatically generated)
   /// ---legacy--- parentClass - parent class, NULL for root class ("Object")
   /// ---legacy--- version -  per class version number, set to FIRST_VERSION + number
   void         init(card32 uniqueIdentifier, const MetaClass *parentClass,
                     card32 classVersion = FIRST_VERSION);
   virtual void copyParentProperties();
   card32       getNumProperties() const;
   void         addProperty(ClassProperty *prop);
   const ClassProperty *getProperty(card32 i) const;
   card32       getNumMethods() const;
   void         addMethod(ClassMethod *m);
   const ClassMethod *getMethod(card32 i) const;
   Object*      newInstance() const; // throws EClassAbstract
   bool         isAbstract() const; // returns newInstanceMethod == NULL
   card32       getUniqueIdentifier() const; // obsolete
   card32       getInstanceSize() const;
   bool         inheritsFrom(const MetaClass* mc) const;
   const string getClassName() const;
   const string getClassDescription() const;
   card32       getClassVersion() const;

   // interface for the last class property
   void actProperty_setUserEdit( bool b );
   void actProperty_setEditReadonly( bool b );
   void actProperty_setStreamProperty( bool b );
	void actProperty_setPropertyName( string name );
   void actProperty_addPropertyEditor( const MetaClass *mc, bool defaultEd = true );
   
   card32 getNumDescendants() const {return (card32)descendants.size();}
   const MetaClass *getDescendant(card32 i) const {return descendants[i];}
   const MetaClass *getParentClass() const {return parentClass;}

   const GUIDescriptor *getGUIDescriptor() const {return guiDescr;}
         GUIDescriptor *getGUIDescriptor() {return guiDescr;}

   inline bool  operator==(MetaClass *c) {return this == c;}

   static const MetaClass* getClassByUniqueIdentifier(card32 uniqueIdentifier);    // obsolete
   /// returns NULL if no matching class is found 
   static const MetaClass* getClassByName(const string className);
   /// returns true if class is present in global class registration table (useful for debugging)
   static bool isClassRegistered(const string className);

	/// calls a class method that does not require arguments
	/// returns true if the method was found and called successfully, false otherwise
	bool callNoArgsMethod(Object *obj, string methodName) const;

   /** creates new instance, uses (default) class creator to display creation dialog if possible */
   //Object *interactivelyCreateNewInstance(TControl *AOwner) const;


   /// call at the end of the application to check whether all classes have been shutdown (check is ommited if not in debug mode, i.e. _DEBUG defined)
   static void checkAllShutdown();

   virtual ~MetaClass();
};




#endif
