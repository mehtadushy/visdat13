//---------------------------------------------------------------------------
#ifndef ClassPropertyH
#define ClassPropertyH
//---------------------------------------------------------------------------
#include <string>
#include <deque>
#include "PTypes.h"
#include "Exceptions.h"
#include "Object.h"
//---------------------------------------------------------------------------


using namespace std;


class InputObjectStream;
class OutputObjectStream;
class PropertyEditor;
class MetaClass;

typedef card32 Version;

class  EUnparsable : public PException {
 public:
   EUnparsable(const string msg) : PException(msg) {}
   virtual string getTypeString() {return "EUnparsable";}
};

class  ClassProperty : public Object {
 private:
   deque<const MetaClass*> editors;
   string cName;
   string description;
   Version introducedInVersion;
   bool userEdit;
   bool editReadonly;
   bool streamProperty;
   const MetaClass* ownerClass;   // Owner of this property
	bool compoundAble;

 protected:
   ClassProperty();
 public:
   /// constructor: use MetaClass::FIRST_VERSION + i to define introduction of property in class revision i
   ClassProperty(const string cName, const Version introducedInVersion);
   virtual void             setDescription(const string description);
   virtual void             addPropertyEditor(const MetaClass *edClass, bool defaultEditor = false);
   virtual void             assign(const Object* obj, CopyContext *context = NULL);

   virtual const string     getTypeName() const = 0;
   virtual const string     getCName() const;
   virtual const string     getDescription() const;
   virtual const Version    getIntroducedInVersion() const;
   virtual void             writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const = 0;
   virtual void             readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const = 0;
   virtual string           toString(const Object* obj, pointer_diff offset) const = 0;
   virtual void             fromString(Object* obj, pointer_diff offset, const string str) const = 0; // throws EUnparsable
   virtual const MetaClass *getPropertyEditor(card32 i) const;
   virtual const MetaClass *getDefaultPropertyEditor() const;
   virtual card32           getNumPropertyEditors() const;
   virtual void             assignProperty(const Object *from, Object *to, pointer_diff offset) const = 0;

   void setUserEdit(bool userEdit);             // suitable for interactive editing, default = true
   void setEditReadonly(bool editReadonly);     // property cannot be changed interactively, default = false
   void setStreamProperty(bool streamProperty); // property will be written in object stream, default = true
   void setOwner(const MetaClass* ownerClass) {this->ownerClass = ownerClass;}
	void setCompoundAble( bool b ) {compoundAble = b;}
   bool getUserEdit() const {return userEdit;}
   bool getEditReadonly() const {return editReadonly;}
   bool getStreamProperty() const {return streamProperty;}
   const MetaClass* getOwner() const {return ownerClass;}
	bool getCompoundAble() const {return compoundAble;}
	void setCName( string name ) {cName = name;}

   virtual ClassProperty* copyClassProperty() = 0;
};




#endif
