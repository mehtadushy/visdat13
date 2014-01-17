//---------------------------------------------------------------------------
#ifndef ElementEditorH
#define ElementEditorH
//---------------------------------------------------------------------------
#include <QtGui/QWidget>
#include "PTypes.h"
//---------------------------------------------------------------------------



class  ElementEditor : public QWidget
{
	Q_OBJECT

private:
	pointer_diff	m_Offset;

public:

	ElementEditor(QWidget * parent=NULL, Qt::WFlags f=0);
	~ElementEditor(void);

	void setOffset( pointer_diff offset ) {m_Offset = offset;}
	pointer_diff getOffset() const {return m_Offset;}

protected:

	/// update editor
	virtual void updateEditor() = 0;
	
	/// emits signal sigModified
	void modified();

public slots:

	void sltUpdate();

signals:

	void sigModified();

};



#endif
