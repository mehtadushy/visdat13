//---------------------------------------------------------------------------
#ifndef ClassInstanceSetH
#define ClassInstanceSetH
//---------------------------------------------------------------------------
#include "Object.h"
#include "PAssert.h"
//---------------------------------------------------------------------------



/// stores instances of classes, one for each class
/// can be queried by class
class ClassInstanceSet {
 private:
	struct Entry {
		const MetaClass *mc;
		Object *obj;
	};

	vector<Entry> entries;

 public:
	/// insert new object. if an object of the same class already exists, it is deleted and replanced
	void insertObject(Object *obj);
	/// check whether an object of a given class is already present
	bool containsObjectOfClass(const MetaClass *mc);
	/// retrieve instance by class. returns null if not found.
	Object *getInstance(const MetaClass *mc);

	card32 getNumInstances() const { return (card32)entries.size(); }
	Object* getInstanceByIndex( const card32 index ){ pAssert( index < entries.size() ); return entries[index].obj; }

	virtual ~ClassInstanceSet();
};




#endif
