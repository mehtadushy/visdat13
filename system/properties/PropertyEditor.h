//---------------------------------------------------------------------------
#ifndef PropertyEditorH
#define PropertyEditorH
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------

class QWidget;



const card32 STD_EDITOR_HEIGHT = 24;

class  PropertyEditor : public Object {
 private:
   static MetaClass *PropertyEditorMetaClass;

protected:
	ClassProperty	*	m_Property;
	Object			*	m_Object;
	pointer_diff		m_Offset;

public:
   PropertyEditor();

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();
	
	/// binds property editor to object property
	/// call setup before calling makeEditor
	virtual void setup( Object * obj, pointer_diff offset, const ClassProperty * prop ) {m_Object = obj; m_Property = (ClassProperty*)prop; m_Offset = offset;}

	/// creates an editor
	virtual QWidget *makeEditor(QWidget * parent, bool readOnly) = 0;	

	/// update editor
	virtual void update() = 0;

//   virtual card32 getPreferredWidth() = 0;	// doesn't make sense -> fixed propertybrowser width by user
   virtual card32 getPreferredHeight() = 0;

   virtual ~PropertyEditor() {}
};




#endif
