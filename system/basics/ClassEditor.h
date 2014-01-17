//---------------------------------------------------------------------------
#ifndef ClassEditorH
#define ClassEditorH
//---------------------------------------------------------------------------
#include "Object.h"
#include "Editor.h"
#include "ObjectViewsTable.h"
//---------------------------------------------------------------------------

class QWidget;


class  ClassEditor : public Object, public Editor {
 private:
   static MetaClass *ClassEditorMetaClass;

 public:

   ClassEditor();

   virtual const MetaClass *getInstanceClass() const;
   static const MetaClass *getClass();
   static void init(const MetaClass *parentClass);
   static void shutdown();

   virtual void update() = 0;
	virtual card32 getPreferredWidth() = 0;
   virtual card32 getPreferredHeight() = 0;

	virtual QWidget * makeEditor( QWidget * parent, Object * obj ) = 0;
	virtual Object * getObject() = 0;

	virtual bool standAloneEditorApplication() {return false;}
};

/**
	declares a class editor
*/
#define MAKE_CLASS_EDITOR_EX( api, editorName, widgetName ) \
class api editorName : public ClassEditor \
{\
private:\
	static MetaClass * editorName##MetaClass;\
   static Object* newInstance();\
\
	widgetName  *m_Widget;\
	Object	*	m_Object;\
\
public:\
   virtual const MetaClass *getInstanceClass() const;\
   static const MetaClass *getClass();\
   static void init(const MetaClass *parentClass);\
   static void shutdown();\
\
	editorName() {m_Widget = NULL;} \
\
   virtual QWidget *makeEditor(QWidget *parent, Object *obj);\
	virtual void apply() {}\
   virtual void update();\
   virtual card32 getPreferredWidth();\
   virtual card32 getPreferredHeight();\
   virtual Object *getObject();\
\
	virtual bool standAloneEditorApplication();\
\
	virtual ~editorName();\
};

#define MAKE_CLASS_EDITOR( api, className ) MAKE_CLASS_EDITOR_EX( api, className##Editor, className##EditorWidget )


/**
	implements a class editor ( param = editorName )
*/
#define IMPLEMENT_CLASS_EDITOR_EX( className, editorName, widgetName, standAlone ) \
MetaClass* editorName::editorName##MetaClass = NULL;\
\
const MetaClass* editorName::getInstanceClass() const {\
   return getClass();\
}\
\
Object* editorName::newInstance() {\
   return new editorName;\
}\
\
const MetaClass* editorName::getClass() {		\
   if (editorName##MetaClass==NULL) {		\
	editorName##MetaClass = new MetaClass( #editorName , "no description.",		\
                                  newInstance, sizeof( editorName ));		\
   }		\
   return editorName##MetaClass;		\
}		\
		\
void editorName::init(const MetaClass *parentClass) {		\
   ((MetaClass*)getClass())->init(parentClass, MetaClass::FIRST_VERSION + 0);		\
   editorName##MetaClass->copyParentProperties();		\
}		\
		\
void editorName::shutdown() {		\
   delete editorName##MetaClass;		\
}		\
void editorName::update() {m_Widget->updateEditor();}\
card32 editorName::getPreferredWidth() {return m_Widget->width();}\
card32 editorName::getPreferredHeight()  {return m_Widget->height();}\
QWidget * editorName::makeEditor( QWidget * parent, Object * obj ) { \
	m_Widget = new widgetName( parent ); \
	m_Object = obj; \
	m_Widget->setup( ( className *)m_Object ); \
	m_Widget->updateEditor(); \
	ObjectViewsTable::bindView(m_Object, this);\
	return m_Widget; }\
Object * editorName::getObject() {return m_Object;}\
\
bool editorName::standAloneEditorApplication() {return standAlone;}\
\
editorName::~editorName() {\
	ObjectViewsTable::releaseView(m_Object, this);\
}

/// use this macro for widget style editor
#define IMPLEMENT_CLASS_EDITOR( className ) IMPLEMENT_CLASS_EDITOR_EX( className, className##Editor , className##EditorWidget, false )
/// use this macro for application style editors
#define IMPLEMENT_CLASS_EDITOR_STANDALONE( className ) IMPLEMENT_CLASS_EDITOR_EX( className, className##Editor , className##EditorWidget, true )



#endif
