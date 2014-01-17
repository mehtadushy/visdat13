//---------------------------------------------------------------------------
#ifndef EditorH
#define EditorH
//---------------------------------------------------------------------------


class Object;

class Editor { /* INTERFACE */
 public:
   virtual Object *getObject() = 0;
   virtual void update() = 0;
   virtual void apply() = 0;
   virtual ~Editor() {}
};



#endif
