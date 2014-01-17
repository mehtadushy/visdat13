//---------------------------------------------------------------------------
#ifndef LegacyClassNameTranslationH
#define LegacyClassNameTranslationH
//---------------------------------------------------------------------------
#include <map>
#include <string>
//---------------------------------------------------------------------------

using namespace std;

class MetaClass;
class Persistent;
class InputObjectStream;

/// This class translates the name old classes into new classes in order to read from old streams.
/// Use the versioning mechanism to provide input handling for legacy classes.
/// Register the handler using the static methods of LegacyClassReadHandler.
class  LegacyClassNameTranslation {
 private:
   static map<string, const MetaClass*> translations;

 public:
   // --- register handlers globally
   /// register a new legacy translator; the object will be subsequently owned by this class.
   static void registerTranslation(string legacyClassName, const MetaClass *currentClass);
   /// query for a legacy read handler; returns NULL if class name is not known.
   static const MetaClass *getCurrentClass(string legacyClassName);
};




#endif
